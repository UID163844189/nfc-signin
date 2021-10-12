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

			Console.WriteLine("请选择要从xx开始进行的操作：\n1：分UID\n2：计算时间");

			int choose = int.Parse(Console.ReadLine());


			switch (choose)
			{
				default:
				case 1:
					Console.WriteLine("请输入日志文件的目录，默认recieve.csv");
					string IncomingString = Console.ReadLine();
					if (IncomingString != string.Empty)
						RecordFilePath = IncomingString;
					SplitUID();
					Calculate();
					break;
				case 2:
					Calculate();
					break;
			}
			goto Main;
		}

		static string RecordFilePath = "recieve.csv";
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
		static void Calculate()
		{
			Console.WriteLine("2");

		}
	}
}
