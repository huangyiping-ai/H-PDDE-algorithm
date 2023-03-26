// main_SP_or_determine.cpp : 针对小规模和大规模实例，对比两阶段随机规划模型和确定性模型。

#include "pch.h"
#include "PDDE.h"
using namespace std;

/*
//小规模实例的主程序
int _tmain(int argc, _TCHAR* argv[])
{
	PDDE* Small_Case = new PDDE();
	std::ofstream ofile("D:\\small_SP_or_determine.txt");
	std::ofstream ofile_allobj("D:\\small_SP_or_determine_allobj.txt");
	std::ofstream ofile_solution("D:\\small_SP_or_determine_solution.txt");

	Small_Case->Demand_Lower = 0;
	Small_Case->Demand_Upper = 0;
	int jobslist[2] = { 10,15 };
	int stageslist[2] = { 2, 5 };
	int machineslist[3] = { 1,2,3 };
	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < 2; j++)
		{
			for (int k = 0; k < 3; k++)
			{

				PDDE::Result result;

				//求解两阶段随机规划模型
				//Small_Case->Call_PDDE_Variable_Heuristic_Neighborhood(jobslist[i], stageslist[j], machineslist[k], result);
				//求解确定性模型
				int avgprocesstime = 4;
				Small_Case->Call_PDDE_Variable_Heuristic_Neighborhood_determine_processtime(jobslist[i], stageslist[j], machineslist[k], avgprocesstime, result);


				//随机产生一种加工时间，计算目标函数值
				vector<PDDE::Individual> message;
				message.clear();
				for (int l = 0; l < 100; l++)
				{
					//产生随机加工时间
					int processtime;
					double randnum = (rand() % 100) * 0.01;
					if (randnum < 0.3)
						processtime = 3;
					else if (randnum >= 0.3 && randnum < 0.7)
						processtime = 4;
					else
						processtime = 5;

					//解码
					for (int n = 0; n < Small_Case->scenarios; n++)
					{
						int jobindex = accumulate(Small_Case->product_types_to_jobs.begin(), Small_Case->product_types_to_jobs.end() - 1, 0);
						for (int y = 0; y < Small_Case->product_types_to_jobs[Small_Case->product_types - 1]; y++)
						{
							Small_Case->processing_time[n][jobindex][Small_Case->stages - 1] = processtime;
							jobindex++;
						}
					}

					PDDE::Individual decode;
					decode.sequence = result.best.sequence;
					Small_Case->Decoding_Individual(decode);

					message.push_back(decode);
				}


				//统计结果
				double bestobjective = message[0].objective;
				double avgobjective = 0;
				double worstobjective = message[0].objective;
				for (int l = 0; l < message.size(); l++)
				{
					if (message[l].objective < bestobjective)
						bestobjective = message[l].objective;
					avgobjective = avgobjective + message[l].objective;
					if (message[l].objective > worstobjective)
						worstobjective = message[l].objective;
				}

				//在文档中记录结果
				ofile << bestobjective << " " << avgobjective / message.size() << " " << worstobjective << " " << endl;

				for (int l = 0; l < message.size(); l++)
					ofile_allobj << message[l].objective << " ";
				ofile_allobj << endl;

				for (int l = 0; l < result.best.sequence.size(); l++)
					ofile_solution << result.best.sequence[l] << " ";
				ofile_solution << endl;

			}
		}
	}

	ofile.close();
	ofile_allobj.close();
	ofile_solution.close();

	system("pause");
	return 0;
}
*/

/*
//大规模实例的主程序
int _tmain(int argc, _TCHAR* argv[])
{
	PDDE* Large_Case = new PDDE();
	std::ofstream ofile("D:\\large_SP_or_determine.txt");
	std::ofstream ofile_allobj("D:\\large_SP_or_determine_allobj.txt");
	std::ofstream ofile_solution("D:\\large_SP_or_determine_solution.txt");

	int jobslist[3] = { 20,50,100 };
	int stageslist[3] = { 2,5,8 };
	int machineslist[2] = { 3,4 };
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			for (int k = 0; k < 2; k++)
			{

				PDDE::Result result;

				//求解两阶段随机规划模型
				//Large_Case->Demand_Lower = 5;
				//Large_Case->Demand_Upper = 9;
				//Large_Case->Call_PDDE_Variable_Heuristic_Neighborhood(jobslist[i], stageslist[j], machineslist[k], result);
				//求解确定性模型
				Large_Case->Demand_Lower = 7;
				Large_Case->Demand_Upper = 7;
				int avgdemand = 7;
				Large_Case->Call_PDDE_Variable_Heuristic_Neighborhood_determine_demand(jobslist[i], stageslist[j], machineslist[k], avgdemand, result);


				//随机产生一种需求，计算目标函数值
				vector<PDDE::Individual> message;
				message.clear();
				for (int l = 0; l < 100; l++)
				{
					//产生随机需求
					int demand;
					double randnum = (rand() % 100) * 0.01;
					if (randnum < 0.1)
						demand = 5;
					else if (randnum >= 0.1 && randnum < 0.3)
						demand = 6;
					else if (randnum >= 0.3 && randnum < 0.7)
						demand = 7;
					else if (randnum >= 0.7 && randnum < 0.9)
						demand = 8;
					else
						demand = 9;

					//解码
					for(int n=0;n< Large_Case->scenarios;n++)
						Large_Case->Demand[n][Large_Case->product_types - 1] = demand;

					PDDE::Individual decode;
					decode.sequence = result.best.sequence;
					Large_Case->Decoding_Individual(decode);

					message.push_back(decode);
				}
				
				
				//统计结果
				double bestobjective = message[0].objective;
				double avgobjective = 0;
				double worstobjective = message[0].objective;
				for (int l = 0; l < message.size(); l++)
				{
					if (message[l].objective < bestobjective)
						bestobjective = message[l].objective;
					avgobjective = avgobjective + message[l].objective;
					if (message[l].objective > worstobjective)
						worstobjective = message[l].objective;
				}

				//在文档中记录结果
				ofile << bestobjective << " " << avgobjective / message.size() << " " << worstobjective << " " << endl;

				for (int l = 0; l < message.size(); l++)
					ofile_allobj << message[l].objective << " ";
				ofile_allobj << endl;

				for (int l = 0; l < result.best.sequence.size(); l++)
					ofile_solution << result.best.sequence[l] << " ";
				ofile_solution << endl;

			}
		}
	}

	ofile.close();
	ofile_allobj.close();
	ofile_solution.close();

	system("pause");
	return 0;
}
*/