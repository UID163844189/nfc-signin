using System.IO;
using System;

namespace AnalyzeSign
{
	class Program
	{
		static void Main(string[] args)
		{
		Main:
			Console.WriteLine("Hello World!");

			//Console.WriteLine("请选择要从xx开始进行的操作：\n1：清除之前分UID的记录\n2：分UID\n3：计算时间");
			Console.WriteLine("请选择要从 xx 开始进行的操作：");
			Console.WriteLine("1：清除之前分 UID 的记录");
			Console.WriteLine("2：分 UID");
			Console.WriteLine("3：UID 转名字");
			Console.WriteLine("4：计算时间");

			string choose = Console.ReadLine();
			if (choose == string.Empty)
			{
				choose = "0";
			}

			switch (int.Parse(choose))
			{
				default:
				case 1:
					Console.WriteLine("清除之前分UID的记录");
					ClearCache();
					Console.WriteLine("完成！");
					goto case 2;
				case 2:
					Console.WriteLine("分UID");
					Console.WriteLine("请输入日志文件的目录，默认recieve.csv");
					string IncomingString = Console.ReadLine();
					if (IncomingString != string.Empty)
						UidFilePath = IncomingString;
					SplitUID();
					Console.WriteLine("完成！");
					goto case 3;
				case 3:
					Console.WriteLine("把 UID 转成名字");
					Console.WriteLine("请输入人员名字文件，默认uidtag.csv");
					IncomingString = Console.ReadLine();
					if (IncomingString != string.Empty)
						RecordFilePath = IncomingString;
					UID2Name();
					goto case 4;
				case 4:
					Calculate();
					break;
			}
			goto Main;
		}

		static string RecordFilePath = "recieve.csv";
		static string UidFilePath = "uidtag.csv";

		static void UID2Name()
		{
			if (!File.Exists(UidFilePath))
				File.Create(UidFilePath).Close();
			string[] UidTag = File.ReadAllLines(UidFilePath);
			string[,] UidTable = new string[2, UidTag.Length];

			for (int i = 0; i < UidTag.Length; i++)
			{
				UidTable[0, i] = UidTag[i].Split(",")[0];
				UidTable[1, i] = UidTag[i].Split(",")[1];
			}

			for (int i = 0; i < UidTag.Length; i++)
			{
				Console.WriteLine(UidTable[0, i] + " -> " + UidTable[1, i]);
			}
		}

		static void SplitUID()
		{
			Console.WriteLine("1");

			if (!Directory.Exists("csv"))
				Directory.CreateDirectory("csv");

			string[] Records = File.ReadAllLines(RecordFilePath);

			foreach (string ThisRecord in Records)
			{
				string UID = ThisRecord.Split(",")[0];
				Console.WriteLine("uid: " + UID);
				//Console.ReadLine();
				if (!File.Exists(@"csv\" + UID + ".csv"))
					File.Create(@"csv\" + UID + ".csv").Close();
				string AlreadyContent = File.ReadAllText(@"csv\" + UID + ".csv");
				string Content = AlreadyContent + ThisRecord + "\n";
				File.WriteAllText(@"csv\" + UID + ".csv", Content);
			}

		}

		static void ClearCache()
		{
			if (Directory.Exists("csv"))
			{
				DirectoryInfo dir = new DirectoryInfo("csv");
				dir.Delete(true);
			}
		}

		static void Calculate()
		{
			Console.WriteLine("2");

			if (!Directory.Exists("csv"))
				Directory.CreateDirectory("csv");

			string[] Files = Directory.GetFiles("csv");
			foreach (string RecordFilePath in Files)
			{
				string[] Record = File.ReadAllLines(RecordFilePath);
				int Time = 0;
				for (int i = 0; i < Record.Length; i++)
				{
					if (Record[i].Split(",")[1] != "1")
					{
						i++;
						continue;
					}

				GetRecord:
					int StartTime = Int32.Parse(Record[i].Split(",")[2]);
					i++;
					//为了防止某些人刷进不刷出
					//特此做防止异常的结构
					if (i >= Record.Length)
					{
						break;
					}
					if (Record[i].Split(",")[1] != "0")
					{
						goto GetRecord;
					}
					int EndTime = Int32.Parse(Record[i].Split(",")[2]);
					Time += EndTime - StartTime;
				}
				Console.WriteLine(RecordFilePath.Split("\\")[1] + "中记录的时间：" + Time.ToString());
			}
		}
	}
}
