/*
 * Copyright (c) 2015, Nordic Semiconductor
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this
 * software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE
 * USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "DLog.h"

#import "FileOperations.h"
#import "IntelHex2BinConverter.h"
#import "Utility.h"

// from components/libaries/crc16
static uint16_t crc16_compute(const uint8_t * p_data, uint32_t size, const uint16_t * p_crc)
{
    uint32_t i;
    uint16_t crc = (p_crc == NULL) ? 0xffff : *p_crc;
    
    for (i = 0; i < size; i++)
    {
        crc  = (unsigned char)(crc >> 8) | (crc << 8);
        crc ^= p_data[i];
        crc ^= (unsigned char)(crc & 0xff) >> 4;
        crc ^= (crc << 8) << 4;
        crc ^= ((crc & 0xff) << 4) << 1;
    }
    
    return crc;
}

@implementation FileOperations

-(FileOperations *) initWithDelegate:(id<FileOperationsDelegate>) delegate blePeripheral:(CBPeripheral *)peripheral bleCharacteristic:(CBCharacteristic *)dfuPacketCharacteristic;
{
    self = [super init];
    if (self)
    {
        self.fileDelegate = delegate;
        self.bluetoothPeripheral = peripheral;
        self.dfuPacketCharacteristic = dfuPacketCharacteristic;
    }
    return self;
}

-(void)openFile:(NSURL *)fileURL
{
    NSData *fileData = [NSData dataWithContentsOfURL:fileURL];
    if (fileData.length > 0) {
        [self processFileData:fileURL];
        [self.fileDelegate onFileOpened:self.binFileSize];
    }
    else {
        DLog(@"Error: file is empty!");
        NSString *errorMessage = [NSString stringWithFormat:@"Error on openning file\n Message: file is empty or not exist"];
        [self.fileDelegate onError:errorMessage];
    }
}

-(BOOL)isFileExtension:(NSString *)fileName fileExtension:(enumFileExtension)fileExtension
{
    if ([[fileName pathExtension] isEqualToString:[Utility stringFileExtension:fileExtension]]) {
        return YES;
    }
    else {
        return NO;
    }
}

-(void)processFileData:(NSURL *)fileURL
{
    NSString *fileName = [[fileURL path] lastPathComponent];
    NSData *fileData = [NSData dataWithContentsOfURL:fileURL];
    if ([self isFileExtension:fileName fileExtension:HEX]) {
        self.binFileData = [IntelHex2BinConverter convert:fileData];
        DLog(@"HexFileSize: %lu and BinFileSize: %lu",(unsigned long)fileData.length,(unsigned long)self.binFileData.length);
    }
    else if ([self isFileExtension:fileName fileExtension:BIN]) {
        self.binFileData = [NSData dataWithContentsOfURL:fileURL];
        DLog(@"BinFileSize: %lu",(unsigned long)self.binFileData.length);
    }
    self.numberOfPackets = ceil((double)self.binFileData.length / (double)PACKET_SIZE);
    self.bytesInLastPacket = (self.binFileData.length % PACKET_SIZE);
    if (self.bytesInLastPacket == 0) {
        self.bytesInLastPacket = PACKET_SIZE;
    }
    DLog(@"Number of Packets %d Bytes in last Packet %d",self.numberOfPackets,self.bytesInLastPacket);

    self.crc16BinFileData = crc16_compute([self.binFileData bytes], (uint32_t)[self.binFileData length], NULL);
    DLog(@"crc16 0x%04x", self.crc16BinFileData);
    
    self.writingPacketNumber = 0;
    self.binFileSize = self.binFileData.length;
}

-(void)writeNextPacket
{
    int percentage = 0;
    for (int index = 0; index<PACKETS_NOTIFICATION_INTERVAL; index++) {
        if (self.writingPacketNumber > self.numberOfPackets-2) {
            DLog(@"writing last packet");
            NSRange dataRange = NSMakeRange(self.writingPacketNumber*PACKET_SIZE, self.bytesInLastPacket);
            NSData *nextPacketData = [self.binFileData subdataWithRange:dataRange];
            DLog(@"writing packet number %d ...",self.writingPacketNumber+1);
            DLog(@"packet data: %@",nextPacketData);
            [self.bluetoothPeripheral writeValue:nextPacketData forCharacteristic:self.dfuPacketCharacteristic type:CBCharacteristicWriteWithoutResponse];
            self.writingPacketNumber++;
            [self.fileDelegate onAllPacketsTranferred];            
            break;
        }
        NSRange dataRange = NSMakeRange(self.writingPacketNumber*PACKET_SIZE, PACKET_SIZE);
        NSData *nextPacketData = [self.binFileData subdataWithRange:dataRange];
        DLog(@"writing packet number %d ...",self.writingPacketNumber+1);
        DLog(@"packet data: %@",nextPacketData);
        [self.bluetoothPeripheral writeValue:nextPacketData forCharacteristic:self.dfuPacketCharacteristic type:CBCharacteristicWriteWithoutResponse];
        percentage = (((double)(self.writingPacketNumber * 20) / (double)(self.binFileSize)) * 100);
        [self.fileDelegate onTransferPercentage:percentage];
        self.writingPacketNumber++;        
    }

}

-(void)setBLEParameters:(CBPeripheral *)peripheral bleCharacteristic:(CBCharacteristic *)dfuPacketCharacteristic
{
    self.bluetoothPeripheral = peripheral;
    self.dfuPacketCharacteristic = dfuPacketCharacteristic;
}

@end
