// main_PDDE+Strategy.cpp :���С��ģ�ʹ��ģʵ��������PDDE�㷨���������ԡ�

#include "pch.h"
#include "PDDE.h"
using namespace std;


//С��ģʵ����������
int _tmain(int argc, _TCHAR* argv[])
{
	PDDE* Small_Case = new PDDE();
	std::ofstream ofile("D:\\small.txt");
	std::ofstream ofile_allobj("D:\\small_allobj.txt");
	std::ofstream ofile_iteration_value("D:\\small_iteration_value.txt");

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

				//�����㷨��һ����������10��
				vector<PDDE::Result> message;
				message.clear();
				for (int l = 0; l < 10; l++)
				{
					PDDE::Result result;
					//����PDDE�㷨+������볤�ȿɱ����+����ʽ�㷨��ʼ��Ⱥ+�����������ԣ���H-PDDE
					Small_Case->Call_PDDE_Variable_Heuristic_Neighborhood(jobslist[i], stageslist[j], machineslist[k], result);
					message.push_back(result);
				}

				//ͳ�ƽ��
				double avgobjective = 0;
				double avgcomputingtime = 0;
				double avggeneration = 0;
				for (int l = 0; l < message.size(); l++)
				{
					avgobjective = avgobjective + message[l].best.objective;
					avgcomputingtime = avgcomputingtime + message[l].computingtime;
					avggeneration = avggeneration + message[l].generation;
				}
				double std = Small_Case->Standard(avgobjective/ message.size(), message);

				//���ĵ��м�¼���
				ofile << avgobjective / message.size() << " " << std << " " << avgcomputingtime / message.size() << " " << avggeneration / message.size() << endl;
				
				for (int l = 0; l < message.size(); l++)
					ofile_allobj << message[l].best.objective << " ";
				ofile_allobj << endl;

				for (int l = 0; l < message.size(); l++)
				{
					for (int n = 0; n < message[l].iteration_value.size(); n++)
						ofile_iteration_value << message[l].iteration_value[n] << " ";
					ofile_iteration_value << endl;
				}

			}
		}
	}

	ofile.close();
	ofile_allobj.close();
	ofile_iteration_value.close();

	system("pause");
	return 0;
}


/*
//���ģʵ����������
int _tmain(int argc, _TCHAR* argv[])
{
	PDDE* Large_Case = new PDDE();
	std::ofstream ofile("D:\\large.txt");
	std::ofstream ofile_allobj("D:\\large_allobj.txt");
	std::ofstream ofile_iteration_value("D:\\large_iteration_value.txt");

	Large_Case->Demand_Lower = 5;
	Large_Case->Demand_Upper = 9;
	int jobslist[3] = { 20,50,100 };
	int stageslist[3] = { 2,5,8 };
	int machineslist[2] = { 3,4 };
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			for (int k = 0; k < 2; k++)
			{

				//�����㷨��һ����������10��
				vector<PDDE::Result> message;
				message.clear();
				for (int l = 0; l < 10; l++)
				{
					PDDE::Result result;
					//����PDDE�㷨+������볤�ȿɱ����+����ʽ�㷨��ʼ��Ⱥ+�����������ԣ���H-PDDE
					//Large_Case->Call_PDDE_Variable_Heuristic_Neighborhood(jobslist[i], stageslist[j], machineslist[k], result);
					message.push_back(result);
				}
				
				//ͳ�ƽ��
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

				//���ĵ��м�¼���
				ofile << avgobjective / message.size() << " " << std << " " << avgcomputingtime / message.size() << " " << avggeneration / message.size() << endl;
				
				for (int l = 0; l < message.size(); l++)
					ofile_allobj << message[l].best.objective << " ";
				ofile_allobj << endl;

				for (int l = 0; l < message.size(); l++)
				{
					for (int n = 0; n < message[l].iteration_value.size(); n++)
						ofile_iteration_value << message[l].iteration_value[n] << " ";
					ofile_iteration_value << endl;
				}

			}
		}
	}

	ofile.close();
	ofile_allobj.close();
	ofile_iteration_value.close();

	system("pause");
	return 0;
}
*/