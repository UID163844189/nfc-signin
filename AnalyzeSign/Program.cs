using System;

namespace AnalyzeSign
{
	class Program
	{
		static void Main(string[] args)
		{
			Console.WriteLine("Hello World!");

			Console.WriteLine("请选择要从xx开始进行的操作：\n1：分UID\n2：计算时间");

			int choose = int.Parse(Console.ReadLine());
			switch (choose)
			{
				default:
				case 1:
					SplitUID();
					Calculate();
					break;
				case 2:
					Calculate();
					break;
			}

		}
		static void SplitUID()
		{
			Console.WriteLine("1");
		}
		static void Calculate()
		{
			Console.WriteLine("2");

		}
	}
}
