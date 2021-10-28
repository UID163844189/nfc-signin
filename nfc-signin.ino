/**********************************
 * 连线方式：
 * -----------------------------------------------------------------------------------------
 *             MFRC522      Arduino       Arduino   Arduino    Arduino          Arduino
 *             Reader/PCD   Uno/101       Mega      Nano v3    Leonardo/Micro   Pro Micro
 * Signal      Pin          Pin           Pin       Pin        Pin              Pin
 * -----------------------------------------------------------------------------------------
 * RST/Reset   RST          9             5         D9         RESET/ICSP-5     RST
 * SPI SS      SDA(SS)      10            53        D10        10               10
 * SPI MOSI    MOSI         11 / ICSP-4   51        D11        ICSP-4           16
 * SPI MISO    MISO         12 / ICSP-1   50        D12        ICSP-1           14
 * SPI SCK     SCK          13 / ICSP-3   52        D13        ICSP-3           15
 */

#include <MFRC522.h>
#include <SPI.h>
//#include <stdio.h>
#define RST_Pin 9  // NFC模块上RST引脚
#define SDA_Pin 10 // NFC模块上SDA引脚
#define beep 3	   // 蜂鸣器
#define SIn 2	   // 进入的按钮
#define SOut 4	   // 出去的按钮
#define DIn 5	   // 进去的灯
#define DOut 6	   // 出去的灯
byte IOStat;
MFRC522 mfrc522(SDA_Pin, RST_Pin); // new一个MFRC522的实例mfrc522
MFRC522::MIFARE_Key key;		   // 卡中的内容，作为缓存（？）
static byte Data[16];			   //数据

void setup()
{
	Serial.begin(115200);
	pinMode(beep, OUTPUT);
	while (!Serial)
		; // 如果没有打开串口就什么都不做(基于ATMEGA32U4 arduino添加)

	//serial.println("boot up");
	SPI.begin();					   // 初始化spi总线
	mfrc522.PCD_Init();				   // 初始化MFRC522卡实例
	delay(4);						   // 可选的延迟。一些开发板初始化后需要更多的时间做好准备，见Readme
	mfrc522.PCD_DumpVersionToSerial(); // 显示细节PCD-MFRC522读卡器的细节

	// 准备键(两个用作键A和键B)
	// 用FFFFFFFFFFFFh，这是从工厂交货的芯片默认的
	for (byte i = 0; i < 6; i++)
	{
		key.keyByte[i] = 0xFF;
	}

	//Serial.println(F("扫描MIFARE Classic PICC演示读和写。"));
	//Serial.print(F("使用键(为A和B):"));
	//dump_byte_array(key.keyByte, MFRC522::MF_KEY_SIZE);
	Serial.println();

	digitalWrite(DOut, 1);
}

#pragma region NFC
void NfcStart()
{
	analogWrite(beep, 255);
	delay(100);
	analogWrite(beep, 0);
}

void NfcEnd()
{
	analogWrite(beep, 192);
	delay(1000);
	analogWrite(beep, 0);
}

void NfcStop()
{
	analogWrite(beep, 255);
	delay(500);
	analogWrite(beep, 0);
}

void NfcWait()
{
	digitalWrite(beep, HIGH);
	delay(50);
	digitalWrite(beep, LOW);
	delay(50);
	digitalWrite(beep, HIGH);
	delay(50);
	digitalWrite(beep, LOW);
	delay(50);
	digitalWrite(beep, HIGH);
	delay(50);
	digitalWrite(beep, LOW);
}
#pragma endregion

void loop()
{
	if (digitalRead(SIn) == HIGH)
	{
		digitalWrite(DOut, 0);
		digitalWrite(DIn, 1);
		IOStat = 1;
	}
	if (digitalRead(SOut) == HIGH)
	{
		digitalWrite(DIn, 0);
		digitalWrite(DOut, 1);
		IOStat = 0;
	}

	NFCBatch();
	String Coming = Serial.readStringUntil("\n");
	if (Coming.startsWith(">Open"))
	{
		Serial.println(">>Open the door");
	}
}

void NFCBatch()
{
	// 如果没有新卡出现在传感器或读卡器就重置循环。这可以节省空闲时整个过程。
	/*for (Serial.print("searching for card..."); !mfrc522.PICC_IsNewCardPresent(); Serial.print("."))
	{
		delay(500);
	}*/
	//NfcWait();

	//Serial.print("searching for card...");

	while (!mfrc522.PICC_IsNewCardPresent())
		return;
	//Serial.println();

	//NfcStart();

	// 选择一个卡
	if (!mfrc522.PICC_ReadCardSerial())
	{
		return;
	}
	// Check for compatibility
	MFRC522::PICC_Type piccType = mfrc522.PICC_GetType(mfrc522.uid.sak);
	if (piccType != MFRC522::PICC_TYPE_MIFARE_MINI && piccType != MFRC522::PICC_TYPE_MIFARE_1K && piccType != MFRC522::PICC_TYPE_MIFARE_4K)
	{
		//Serial.println(F("This sample only works with MIFARE Classic cards."));
		//return;
	}

	//Serial.println("please select: ");
	//Serial.println("1: AutoRead: print all data in the PICC");
	//Serial.println("2: Detail: echo the details of the PICC");
	//Serial.println("3: Read: print data in selected block");
	for (int i = 0; i < 16; i++)
	{
		Data[i] = i;
	}
	/*Data[] = {
		0x01, 0x02, 0x03, 0x04, //  1,  2,   3,  4,
		0x05, 0x06, 0x07, 0x08, //  5,  6,   7,  8,
		0x09, 0x0a, 0xfe, 0x0b, //  9, 10, 254, 11,
		0x0c, 0x0d, 0x0e, 0x0f	// 12, 13, 14, 15
	};*/
	//for (; !Serial.available() > 0;)
	//	;

	//ReadDebugInfo();
	/*
	for (;;)
	{
		if (Serial.available()>0)
		{
			AutoReadCard();
			break;
		}
		if (digitalRead(2) == HIGH)
		{
			Serial.println("high");
			ReadCard();
			break;
		}
		
		
	}*/

	//char cmd = Serial.read();
	int cmd = 4;
	//delay(500);
	//Serial.print("your cmd is ");
	//Serial.println(cmd);
	//NfcStart();
	switch (cmd)
	{
	case 1:
		ReadDebugInfo();
		break;
	case 2:
		Detail();
		break;
	case 3:
		ReadCard();
		break;
	case 4:
		SendInfo();
		break;
	default:
		break;
	}
	NfcStop();
}

void SendInfo()
{
	/*
	{
		"Status":"0",
		"UID":"2C 9C 0F 32", 
		"IO":"0"
	}
	*/

	/*
	Serial.print("{\"Status\":\"0\",\"UID\":\"");
	dump_byte_array(mfrc522.uid.uidByte, mfrc522.uid.size);
	Serial.print("\", \"IO\":\"");
	Serial.print(IOStat);
	Serial.println("\"}");
	*/

	// AABB,1,
	dump_byte_array(mfrc522.uid.uidByte, mfrc522.uid.size);
	Serial.print(",");
	Serial.print(IOStat);
	Serial.println(",");
}

void WriteCard(byte Block)
{

	MFRC522::StatusCode status;
	Serial.print(F("Writing data into block "));
	Serial.print(Block);
	Serial.println(F(" ..."));
	dump_byte_array(Data, 16);
	Serial.println();
	status = (MFRC522::StatusCode)mfrc522.MIFARE_Write(Block, Data, 16);
	if (status != MFRC522::STATUS_OK)
	{
		Serial.print(F("MIFARE_Write() failed: "));
		Serial.println(mfrc522.GetStatusCodeName(status));
	}
	Serial.println();
}

String ReadDebugInfo()
{
	// 转储卡片的调试信息。PICC_HaltA()是自动调用的
	mfrc522.PICC_DumpToSerial(&(mfrc522.uid));
}

void Detail()
{
	// 展示PICC的一些细节(即:标签/卡)
	Serial.print(F("Card UID:"));
	dump_byte_array(mfrc522.uid.uidByte, mfrc522.uid.size);
	Serial.println();
	Serial.print(F("PICC type: "));
	MFRC522::PICC_Type piccType = mfrc522.PICC_GetType(mfrc522.uid.sak);
	Serial.println(mfrc522.PICC_GetTypeName(piccType));
}

void ReadCard()
{
	Serial.println("please insert block (from 0 to 63) >");
	for (; !Serial.available();)
		;
	//char BlockAddr = Serial.read();
	//Serial.parseInt();
	byte BlockAddr = 4;
	MFRC522::StatusCode status;
	byte buffer[18];
	byte size = sizeof(buffer);

	// Read data from the block
	Serial.print(F("Reading data from block "));
	Serial.print(BlockAddr);
	Serial.println(F(" ..."));
	status = (MFRC522::StatusCode)mfrc522.MIFARE_Read(BlockAddr, buffer, &size);
	if (status != MFRC522::STATUS_OK)
	{
		Serial.print(F("MIFARE_Read() failed: "));
		Serial.println(mfrc522.GetStatusCodeName(status));
	}
	Serial.print(F("Data in block "));
	Serial.print(BlockAddr);
	Serial.println(F(":"));
	dump_byte_array(buffer, 16);
	Serial.println();
	Serial.println();
}


void dump_byte_array(byte *buffer, byte bufferSize)
{
	for (byte i = 0; i < bufferSize; i++)
	{
		Serial.print(buffer[i] < 0x10 ? " 0" : " ");
		Serial.print(buffer[i], HEX);
	}
}