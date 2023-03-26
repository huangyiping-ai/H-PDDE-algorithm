// main_Heuristic_or_PDDE.cpp : 针对小规模和大规模实例，调用分派规则和VND结合的启发式算法、PDDE算法。

#include "pch.h"
#include "PDDE.h"
using namespace std;

/*
//小规模实例的主程序
int _tmain(int argc, _TCHAR* argv[])
{
	PDDE* Small_Case = new PDDE();
	std::ofstream ofile("D:\\small.txt");
	std::ofstream ofile_allobj("D:\\small_allobj.txt");
	
	int jobslist[2] = {10,15};
	int stageslist[2] = {2, 5};
	int machineslist[3] = {1,2,3};
	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < 2; j++)
		{
			for (int k = 0; k < 3; k++)
			{

				//运行算法，一个例子运行10次
				vector<PDDE::Result> message;
				message.clear();
				for (int l = 0; l < 10; l++)
				{
					PDDE::Result result;
					Small_Case->Call_Heuristic(jobslist[i], stageslist[j], machineslist[k], result);//分派规则和VND结合的启发式算法
					//Small_Case->Call_PDDE(jobslist[i], stageslist[j], machineslist[k], result);//PDDE算法
					message.push_back(result);
				}

				//统计结果
				double avgobjective = 0;
				double avgcomputingtime = 0;
				double avggeneration = 0;
				for (int l = 0; l < message.size(); l++)
				{
					avgobjective = avgobjective + message[l].best.objective;
					avgcomputingtime = avgcomputingtime + message[l].computingtime;
					avggeneration = avggeneration +message[l].generation;
				}
				double std = Small_Case->Standard(avgobjective/ message.size(), message);

				//在文档中记录结果
				ofile << avgobjective / message.size() << " " << std << " " << avgcomputingtime / message.size() << " " << avggeneration / message.size() << endl;
				
				for (int l = 0; l < message.size(); l++)
					ofile_allobj << message[l].best.objective << " ";
				ofile_allobj << endl;

			}
		}
	}

	ofile.close();
	ofile_allobj.close();

	system("pause");
	return 0;
}
*/

/*
//大规模实例的主程序
int _tmain(int argc, _TCHAR* argv[])
{
	PDDE* Large_Case = new PDDE();
	std::ofstream ofile("D:\\large.txt");
	std::ofstream ofile_allobj("D:\\large_allobj.txt");

	Large_Case->Demand_Lower = 5;
	Large_Case->Demand_Upper = 9;
	int jobslist[3] = {20,50,100};
	int stageslist[3] = {2,5,8};
	int machineslist[2] = {3,4};
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			for (int k = 0; k < 2; k++)
			{

				//运行算法，一个例子运行10次
				vector<PDDE::Result> message;
				message.clear();
				for (int l = 0; l < 10; l++)
				{
					clock_t start, stop;//计时器
					start = clock();//开始计时

					vector<PDDE::Result> resultlist(Large_Case->Demand_Upper - Large_Case->Demand_Lower+1);
					for (int n = 0; n <= Large_Case->Demand_Upper - Large_Case->Demand_Lower; n++)
						Large_Case->Call_Heuristic(jobslist[i] + Large_Case->Demand_Lower + n, stageslist[j], machineslist[k], resultlist[n]);//分派规则和VND结合的启发式算法
						//Large_Case->Call_PDDE(jobslist[i] + Large_Case->Demand_Lower + n, stageslist[j], machineslist[k], resultlist[n]);//PDDE算法

					stop = clock();//结束计时
					double sum_computingtime = (double)(stop - start) / CLOCKS_PER_SEC;//计算时间

					//计算最优结果
					int sum_generation = 0;
					PDDE::Individual best_person = resultlist[0].best;
					for (int n = 0; n <= Large_Case->Demand_Upper - Large_Case->Demand_Lower; n++)
					{
						sum_generation = sum_generation + resultlist[n].generation;
						if (resultlist[n].best.objective < best_person.objective)
							best_person = resultlist[n].best;
					}

					PDDE::Result last_result;
					last_result.computingtime = sum_computingtime;
					last_result.generation = sum_generation;
					last_result.best = best_person;

					message.push_back(last_result);
				}

				//统计结果
				double avgobjective = 0;
				double avgcomputingtime = 0;
				double avggeneration = 0;
				for (int l = 0; l < message.size(); l++)
				{
					avgobjective = avgobjective + message[l].best.objective;
					avgcomputingtime = avgcomputingtime + message[l].computingtime;
					avggeneration = avggeneration + message[l].generation;
				}
				double std = Large_Case->Standard(avgobjective/ message.size(), message);

				//在文档中记录结果
				ofile << avgobjective / message.size() << " " << std << " " << avgcomputingtime / message.size() << " " << avggeneration / message.size() << endl;
				
				for (int l = 0; l < message.size(); l++)
					ofile_allobj << message[l].best.objective << " ";
				ofile_allobj << endl;

			}
		}
	}

	ofile.close();
	ofile_allobj.close();

	system("pause");
	return 0;
}
*/