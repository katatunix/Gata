using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using System.IO;

//Konv.exe . -f tga png -p IMAGE -v g_pszImagesList -h headerfile.h -c source.cpp

namespace Konv
{
	class Program
	{
		static List<string> extList = new List<string>();
		static string prefix;
		static string variable;
		static string hfile;
		static string cppfile;

		static void Main(string[] args)
		{
			if (args.Length == 0)
			{
				Console.WriteLine("Usage: Konv.exe <path> -f <list of extensions> -p <prefix> -v <array varible name> -h <headerfile> -c <sourcefile>");
				Console.WriteLine("Example: Konv.exe . -f tga png -p IMAGE -v g_pszImagesList -h headerfile.h -c source.cpp");
				return;
			}

			// Parse args
			String path = args[0];
			int index = 2;
			for (; index < args.Length; index++)
			{
				if (args[index] == "-p") break;
				extList.Add(args[index]);
			}
			index++;
			prefix = args[index];
			index += 2;
			variable = args[index];
			index += 2;
			hfile = args[index];
			index += 2;
			cppfile = args[index];

			//
			string[] list1 = Directory.GetFiles(path, "*", SearchOption.AllDirectories);
			List<string> list2 = new List<string>();
			for (int i = 0; i < list1.Length; i++)
			{
				string str = list1[i];
				string ext = Path.GetExtension(str);
				if (ext.Length <= 0) continue;

				ext = ext.Substring(1); // remove the dot

				if (checkExt(ext))
				{
					if (str.StartsWith(".\\"))
					{
						str = str.Substring(2);
					}
					str = str.Replace('\\', '/');
					list2.Add(str);
				}
			}

			StreamWriter sw = new StreamWriter(hfile, true);
			sw.WriteLine();
			sw.WriteLine("/////////////////////////////////////////// auto-generated");
			for (int i = 0; i < list2.Count; i++)
			{
				sw.WriteLine("#define " + prefix + "_" + konv(list2[i]) + " " + i);
			}
			sw.WriteLine("#define COUNT_" + prefix + " " + list2.Count);
			sw.Close();

			sw = new StreamWriter(cppfile, true);
			sw.WriteLine();
			sw.WriteLine("/////////////////////////////////////////// auto-generated");
			sw.WriteLine("const char* " + variable + "[] =");
			sw.WriteLine("{");
			for (int i = 0; i < list2.Count; i++)
			{
				sw.WriteLine("\t\"" + list2[i] + "\",");
			}
			sw.WriteLine("};");
			sw.Close();

			Console.WriteLine("[ Konv ][ OK ]");
		}

		static bool checkExt(string ext)
		{
			for (int i = 0; i < extList.Count; i++)
			{
				if (extList[i] == ext)
				{
					return true;
				}
			}
			return false;
		}

		static string konv(string filepath)
		{
			return filepath.Replace('/', '_').Replace('.', '_');
		}
	}
}
