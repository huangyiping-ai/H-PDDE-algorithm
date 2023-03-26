//PDDE.cpp��PDDE�㷨����

#include "pch.h"
#include "PDDE.h"

PDDE::PDDE(void)
{
}

PDDE::~PDDE(void)
{
}

void PDDE::Read_Data()
{
	
	//С��ģʵ��������
	PDDE::scenarios = 3;
	//PDDE::Demand_Lower = 0;
	//PDDE::Demand_Upper = 0;
	//PDDE::jobs = 10;//ȡ{10,15}
	//PDDE::stages = 2;//ȡ{2,5}
	//PDDE::machines = 1;//ȡ{1,2,3}
	PDDE::product_types = 3;

	PDDE::product_types_to_jobs.clear();
	if(PDDE::jobs == 10)
	{
		PDDE::product_types_to_jobs.push_back(3);
		PDDE::product_types_to_jobs.push_back(3);
		PDDE::product_types_to_jobs.push_back(4);
	}
	if (PDDE::jobs == 15)
	{
		PDDE::product_types_to_jobs.push_back(4);
		PDDE::product_types_to_jobs.push_back(5);
		PDDE::product_types_to_jobs.push_back(6);
	}
	
	PDDE::probability.clear();
	PDDE::probability.push_back(0.3);
	PDDE::probability.push_back(0.4);
	PDDE::probability.push_back(0.3);

	//��һ�ֲ�Ʒ�����ڽ׶�k�ļӹ�ʱ��
	vector<int> processing_time_product_types_top;
	processing_time_product_types_top.clear();
	processing_time_product_types_top.push_back(1);
	processing_time_product_types_top.push_back(2);
	processing_time_product_types_top.push_back(3);
	processing_time_product_types_top.push_back(3);
	processing_time_product_types_top.push_back(2);
	//��Ʒ����c�ڽ׶�k�ļӹ�ʱ��
	vector<vector<int>> processing_time_product_types(PDDE::product_types, vector<int>(PDDE::stages, 0));
	for (int i = 0; i < PDDE::stages; i++)
	{
		int randtime = processing_time_product_types_top[i];
		for (int j = 0; j < PDDE::product_types; j++)
		{
			processing_time_product_types[j][i] = randtime;
			randtime++;
		}
	}
	//����s�����һ�ֲ�Ʒ����c�����һ���׶�k�ļӹ�ʱ��
	PDDE::processing_time_uncertain.clear();
	PDDE::processing_time_uncertain.push_back(3);
	PDDE::processing_time_uncertain.push_back(4);
	PDDE::processing_time_uncertain.push_back(5);
	//����s�²�Ʒ����c�ڽ׶�k�ļӹ�ʱ��
	vector<vector<vector<int>>> processing_time_product_types_scenarios;
	processing_time_product_types_scenarios.clear();
	for (int i = 0; i < PDDE::scenarios; i++)
	{
		processing_time_product_types[PDDE::product_types - 1][PDDE::stages - 1] = PDDE::processing_time_uncertain[i];
		processing_time_product_types_scenarios.push_back(processing_time_product_types);
	}
	//����s����ҵj�ڽ׶�k�ļӹ�ʱ��
	vector<vector<vector<int>>> processing_time1(PDDE::scenarios, vector<vector<int>>(PDDE::jobs, vector<int>(PDDE::stages, 0)));
	for (int i = 0; i < PDDE::scenarios; i++)
	{
		for (int j = 0; j < PDDE::stages; j++)
		{
			int jobindex = 0;
			for (int k = 0; k < PDDE::product_types; k++)
			{
				for (int l = 0; l < PDDE::product_types_to_jobs[k]; l++)
				{
					processing_time1[i][jobindex][j] = processing_time_product_types_scenarios[i][k][j];
					jobindex++;
				}
			}
		}
	}
	PDDE::processing_time = processing_time1;
	
	//��Ʒ����c��������
	vector<int> Demand_product_types = PDDE::product_types_to_jobs;
	//����s�²�Ʒ����c��������
	PDDE::Demand.clear();
	for (int i = 0; i < PDDE::scenarios; i++)
		PDDE::Demand.push_back(Demand_product_types);

	//��Ʒ����c�ĵ�����
	vector<int> due_date_product_types;
	due_date_product_types.clear();
	due_date_product_types.push_back(10);
	due_date_product_types.push_back(20);
	due_date_product_types.push_back(30);
	//��ҵj�ĵ����գ���Ʒ������ͬ����ҵ������Ҳ��ͬ
	//PDDE::due_date.clear();
	//for (int i = 0; i < PDDE::product_types; i++)
	//{
	//	for (int j = 0; j < PDDE::product_types_to_jobs[i]; j++)
	//		PDDE::due_date.push_back(due_date_product_types[i]);
	//}
	//��ҵj�ĵ����գ�����һ������
	int due_date_sample[30] = {10,20,30,20,30,10,30,10,20,20,10,30,10,30,10,20,20,30,10,30,10,20,30,30,20,10,30,30,20,20};
	PDDE::due_date.clear();
	for (int i = 0; i < 30; i++)
		PDDE::due_date.push_back(due_date_sample[i]);

	PDDE::unit_processing_cost.clear();
	PDDE::unit_processing_cost.push_back(30);
	PDDE::unit_processing_cost.push_back(40);
	PDDE::unit_processing_cost.push_back(50);

	PDDE::unit_holding_cost.clear();
	PDDE::unit_holding_cost.push_back(10);
	PDDE::unit_holding_cost.push_back(20);
	PDDE::unit_holding_cost.push_back(30);

	PDDE::unit_external_production_cost.clear();
	PDDE::unit_external_production_cost.push_back(300);
	PDDE::unit_external_production_cost.push_back(400);
	PDDE::unit_external_production_cost.push_back(500);

	//��Ʒ����c�ĵ�λ�����깤�ɱ�
	vector<double> unit_delayed_completion_product_types;
	unit_delayed_completion_product_types.clear();
	unit_delayed_completion_product_types.push_back(1);
	unit_delayed_completion_product_types.push_back(2);
	unit_delayed_completion_product_types.push_back(3);
	//��ҵj�ĵ�λ�����깤�ɱ�
	PDDE::unit_delayed_completion_cost.clear();
	for (int i = 0; i < PDDE::product_types; i++)
	{
		for (int j = 0; j < PDDE::product_types_to_jobs[i]; j++)
			PDDE::unit_delayed_completion_cost.push_back(unit_delayed_completion_product_types[i]);
	}

	PDDE::NP = 50;
	PDDE::Gmax = 100;
	PDDE::F = 0.9;
	PDDE::CR = 0.5;
	PDDE::TP=1;
	
	
	/*
	//���ģʵ��������
	PDDE::scenarios = 5;
	//PDDE::Demand_Lower = 5;
	//PDDE::Demand_Upper = 9;
	//PDDE::jobs = 20;//ȡ{20,50,100}
	//PDDE::stages = 2;//ȡ{2,5,8}
	//PDDE::machines = 3;//ȡ{3,4}
	PDDE::product_types = 6;

	PDDE::product_types_to_jobs.clear();
	if (PDDE::jobs >= 20+ PDDE::Demand_Lower && PDDE::jobs <= 20 + PDDE::Demand_Upper)
	{
		PDDE::product_types_to_jobs.push_back(4);
		PDDE::product_types_to_jobs.push_back(4);
		PDDE::product_types_to_jobs.push_back(4);
		PDDE::product_types_to_jobs.push_back(4);
		PDDE::product_types_to_jobs.push_back(4);
		PDDE::product_types_to_jobs.push_back(PDDE::jobs - 20);
	}
	if (PDDE::jobs >= 50 + PDDE::Demand_Lower && PDDE::jobs <= 50 + PDDE::Demand_Upper)
	{
		PDDE::product_types_to_jobs.push_back(10);
		PDDE::product_types_to_jobs.push_back(10);
		PDDE::product_types_to_jobs.push_back(10);
		PDDE::product_types_to_jobs.push_back(10);
		PDDE::product_types_to_jobs.push_back(10);
		PDDE::product_types_to_jobs.push_back(PDDE::jobs - 50);
	}
	if (PDDE::jobs >= 100 + PDDE::Demand_Lower && PDDE::jobs <= 100 + PDDE::Demand_Upper)
	{
		PDDE::product_types_to_jobs.push_back(20);
		PDDE::product_types_to_jobs.push_back(20);
		PDDE::product_types_to_jobs.push_back(20);
		PDDE::product_types_to_jobs.push_back(20);
		PDDE::product_types_to_jobs.push_back(20);
		PDDE::product_types_to_jobs.push_back(PDDE::jobs - 100);
	}

	PDDE::probability.clear();
	PDDE::probability.push_back(0.1);
	PDDE::probability.push_back(0.2);
	PDDE::probability.push_back(0.4);
	PDDE::probability.push_back(0.2);
	PDDE::probability.push_back(0.1);
	
	//��һ�ֲ�Ʒ�����ڽ׶�k�ļӹ�ʱ��
	vector<int> processing_time_product_types_top;
	processing_time_product_types_top.clear();
	processing_time_product_types_top.push_back(1);
	processing_time_product_types_top.push_back(2);
	processing_time_product_types_top.push_back(3);
	processing_time_product_types_top.push_back(3);
	processing_time_product_types_top.push_back(2);
	processing_time_product_types_top.push_back(1);
	processing_time_product_types_top.push_back(2);
	processing_time_product_types_top.push_back(3);
	//��Ʒ����c�ڽ׶�k�ļӹ�ʱ��
	vector<vector<int>> processing_time_product_types(PDDE::product_types, vector<int>(PDDE::stages, 0));
	for (int i = 0; i < PDDE::stages; i++)
	{
		int randtime = processing_time_product_types_top[i];
		for (int j = 0; j < PDDE::product_types; j++)
		{
			processing_time_product_types[j][i] = randtime;
			randtime++;
		}
	}
	//����s�²�Ʒ����c�ڽ׶�k�ļӹ�ʱ��
	vector<vector<vector<int>>> processing_time_product_types_scenarios;
	processing_time_product_types_scenarios.clear();
	for (int i = 0; i < PDDE::scenarios; i++)
		processing_time_product_types_scenarios.push_back(processing_time_product_types);
	//����s����ҵj�ڽ׶�k�ļӹ�ʱ��
	vector<vector<vector<int>>> processing_time1(PDDE::scenarios, vector<vector<int>>(PDDE::jobs, vector<int>(PDDE::stages, 0)));
	for (int i = 0; i < PDDE::scenarios; i++)
	{
		for (int j = 0; j < PDDE::stages; j++)
		{
			int jobindex = 0;
			for (int k = 0; k < PDDE::product_types; k++)
			{
				for (int l = 0; l < PDDE::product_types_to_jobs[k]; l++)
				{
					processing_time1[i][jobindex][j] = processing_time_product_types_scenarios[i][k][j];
					jobindex++;
				}
			}
		}
	}
	PDDE::processing_time = processing_time1;

	//����s�����һ�ֲ�Ʒ���͵�������
	PDDE::Demand_uncertain.clear();
	PDDE::Demand_uncertain.push_back(5);
	PDDE::Demand_uncertain.push_back(6);
	PDDE::Demand_uncertain.push_back(7);
	PDDE::Demand_uncertain.push_back(8);
	PDDE::Demand_uncertain.push_back(9);
	//��Ʒ����c��������
	vector<int> Demand_product_types = PDDE::product_types_to_jobs;
	//����s�²�Ʒ����c��������
	PDDE::Demand.clear();
	for (int i = 0; i < PDDE::scenarios; i++)
	{
		Demand_product_types[PDDE::product_types - 1] = PDDE::Demand_uncertain[i];
		PDDE::Demand.push_back(Demand_product_types);

	}
	
	//��Ʒ����c�ĵ�����
	vector<int> due_date_product_types;
	due_date_product_types.clear();
	due_date_product_types.push_back(15);
	due_date_product_types.push_back(20);
	due_date_product_types.push_back(25);
	due_date_product_types.push_back(30);
	due_date_product_types.push_back(35);
	due_date_product_types.push_back(40);
	//��ҵj�ĵ����գ���Ʒ������ͬ����ҵ������Ҳ��ͬ
	//PDDE::due_date.clear();
	//for (int i = 0; i < PDDE::product_types; i++)
	//{
	//	for (int j = 0; j < PDDE::product_types_to_jobs[i]; j++)
	//		PDDE::due_date.push_back(due_date_product_types[i]);
	//}
	//��ҵj�ĵ����գ�����һ������
	int due_date_sample[200] = {40,35,30,25,20,15,15,25,35,40,35,40,20,25,30,15,35,40,30,20,  40,35,30,25,20,15,15,25,35,40,35,40,20,25,30,15,35,40,30,20, 
	                            40,35,30,25,20,15,15,25,35,40,35,40,20,25,30,15,35,40,30,20,  40,35,30,25,20,15,15,25,35,40,35,40,20,25,30,15,35,40,30,20,
								40,35,30,25,20,15,15,25,35,40,35,40,20,25,30,15,35,40,30,20,  40,35,30,25,20,15,15,25,35,40,35,40,20,25,30,15,35,40,30,20,
								40,35,30,25,20,15,15,25,35,40,35,40,20,25,30,15,35,40,30,20,  40,35,30,25,20,15,15,25,35,40,35,40,20,25,30,15,35,40,30,20,
								40,35,30,25,20,15,15,25,35,40,35,40,20,25,30,15,35,40,30,20,  40,35,30,25,20,15,15,25,35,40,35,40,20,25,30,15,35,40,30,20
								};
	PDDE::due_date.clear();
	for (int i = 0; i < 200; i++)
		PDDE::due_date.push_back(due_date_sample[i]);

	PDDE::unit_processing_cost.clear();
	PDDE::unit_processing_cost.push_back(30);
	PDDE::unit_processing_cost.push_back(35);
	PDDE::unit_processing_cost.push_back(40);
	PDDE::unit_processing_cost.push_back(45);
	PDDE::unit_processing_cost.push_back(50);
	PDDE::unit_processing_cost.push_back(55);

	PDDE::unit_holding_cost.clear();
	PDDE::unit_holding_cost.push_back(10);
	PDDE::unit_holding_cost.push_back(15);
	PDDE::unit_holding_cost.push_back(20);
	PDDE::unit_holding_cost.push_back(25);
	PDDE::unit_holding_cost.push_back(30);
	PDDE::unit_holding_cost.push_back(35);

	PDDE::unit_external_production_cost.clear();
	PDDE::unit_external_production_cost.push_back(300);
	PDDE::unit_external_production_cost.push_back(350);
	PDDE::unit_external_production_cost.push_back(400);
	PDDE::unit_external_production_cost.push_back(450);
	PDDE::unit_external_production_cost.push_back(500);
	PDDE::unit_external_production_cost.push_back(550);

	//��Ʒ����c�ĵ�λ�����깤�ɱ�
	vector<double> unit_delayed_completion_product_types;
	unit_delayed_completion_product_types.clear();
	unit_delayed_completion_product_types.push_back(1);
	unit_delayed_completion_product_types.push_back(1.5);
	unit_delayed_completion_product_types.push_back(2);
	unit_delayed_completion_product_types.push_back(2.5);
	unit_delayed_completion_product_types.push_back(3);
	unit_delayed_completion_product_types.push_back(3.5);
	//��ҵj�ĵ�λ�����깤�ɱ�
	PDDE::unit_delayed_completion_cost.clear();
	for (int i = 0; i < PDDE::product_types; i++)
	{
		for (int j = 0; j < PDDE::product_types_to_jobs[i]; j++)
			PDDE::unit_delayed_completion_cost.push_back(unit_delayed_completion_product_types[i]);
	}

	PDDE::NP = 50;
	PDDE::Gmax = 100;
	PDDE::F = 0.9;
	PDDE::CR = 0.5;
	PDDE::TP = 2;
	*/
}

void PDDE::Initial_Population(vector<Individual>& pop, int jobnum,int NPnum)
{
	//��ʼ����ҵ���У�Ϊjobnum-1��������������
	vector<int> index;
	for (int i = 0; i < jobnum; i++)
	{
		index.push_back(i);
	}

	//�����������
	pop.clear();
	srand((unsigned)time(NULL));
	for (int i = 0; i < NPnum; i++)
	{
		PDDE::Individual xi_init;
		random_shuffle(index.begin(), index.end());
		xi_init.sequence = index;
		PDDE::Decoding_Individual(xi_init);
		pop.push_back(xi_init);
	}
}

void PDDE::Initial_Population_Variable(vector<Individual>& pop, int jobnum, int Lower, int Upper, int NPnum)
{
	int sub_pop=Upper - Lower + 1;//�ֳ�Upper - Lower + 1������Ⱥ
	int sub_pop_NP = round(NPnum / sub_pop);//ÿ������Ⱥ������

	//��ʼ����ҵ����
	vector<int> index;
	for (int i = 0; i < jobnum + Lower; i++)
	{
		index.push_back(i);
	}

	//�����������
	pop.clear();
	srand((unsigned)time(NULL));
	for (int i = 0; i < sub_pop; i++)
	{
		for (int j = 0; j < sub_pop_NP; j++)
		{
			PDDE::Individual xi_init;
			random_shuffle(index.begin(), index.end());
			xi_init.sequence = index;
			PDDE::Decoding_Individual(xi_init);
			pop.push_back(xi_init);
		}
		index.push_back(index.size());
	}
}

void PDDE::Initial_Population_Variable_Heuristic(vector<Individual>& pop, int jobnum, int Lower, int Upper, int NPnum)
{
	int sub_pop = Upper - Lower + 1;//�ֳ�Upper - Lower + 1������Ⱥ
	int sub_pop_NP = round(NPnum / sub_pop);//ÿ������Ⱥ������

	//��ʼ����ҵ����
	vector<int> index;
	for (int i = 0; i < jobnum + Lower; i++)
	{
		index.push_back(i);
	}

	//�����������
	pop.clear();
	srand((unsigned)time(NULL));
	for (int i = 0; i < sub_pop; i++)
	{	
		//���÷��ɹ����VND��ϵ�����ʽ�㷨��ȡ��ʼ��
		vector<int> assign_index = index;
		vector<int> current_due_date{ &PDDE::due_date[0],&PDDE::due_date[0] + assign_index.size() };
		PDDE::EDD_LPT(current_due_date, assign_index);

		PDDE::Individual best;
		best.sequence = assign_index;
		PDDE::Decoding_Individual(best);

		int k = 0;
		while (k < 2)
		{
			PDDE::Individual newbest = PDDE::Find_Neighborhood(k, best.sequence);

			if (newbest.objective < best.objective)
			{
				best = newbest;
				k = 0;
			}
			else
				k = k + 1;
		}

		//�Ŷ���������Ⱥ
		for (int j = 0; j < sub_pop_NP; j++)
		{
			vector<int> randindex;
			if ((rand() % 100) * 0.01 < 0.5)
				randindex = PDDE::Swap(best.sequence);
			else
				randindex = PDDE::Insert(best.sequence);

			PDDE::Individual xi_init;
			xi_init.sequence = randindex;
			PDDE::Decoding_Individual(xi_init);
			pop.push_back(xi_init);
		}

		index.push_back(index.size());
	}
}

void PDDE::Decoding_Individual(Individual& person)
{
	double expect_makespan = 0;
	double expect_total_cost = 0;
	for (int i = 0; i < PDDE::scenarios; i++)
	{
		PDDE::Makespan(person, PDDE::stages, PDDE::machines, PDDE::processing_time[i]);
		PDDE::Total_Cost(person, PDDE::Demand[i], person.lasttime[i]);
		expect_makespan = expect_makespan + PDDE::probability[i]*person.makespan[i];
		expect_total_cost = expect_total_cost + PDDE::probability[i] * person.total_cost[i];
	}
	person.expect_makespan = expect_makespan;
	person.expect_total_cost = expect_total_cost;
	person.objective = 0.7 * expect_makespan + 0.3 * expect_total_cost;
}

/********************
���ܣ��������깤�ȼӹ�ԭ�򣬵õ�ÿ����ҵ��ÿ���׶ε�ÿ̨�����ϵĿ�ʼ����ɣ�ʱ�䣬�Լ�������
���룺
person ����
ordernumber �׶���
parallel ÿ���׶εĲ��л�����
usetime ÿ����ҵ��ÿ���׶εļӹ�ʱ��
��δ���ı��������ǲ��ձ��˵Ĵ���
*********************/
void PDDE::Makespan(Individual& person,int ordernumber, int parallel, vector<vector<int>> usetime)
{
    int workpiecesnumber = person.sequence.size();//��ҵ��
	vector<int> temp1 = person.sequence;//��¼��ǰ�׶���ҵ�����ȼ�����һ�׶ε����ȼ�Ϊ����
	vector<int> temp2(workpiecesnumber,0);//��¼��ǰ�׶���ҵ���깤ʱ�䣬�����깤ʱ���Ⱥ�ȷ����һ�׶���ҵ�����ȼ�
	vector<int> temp3(workpiecesnumber,0);//��¼��һ�׶���ҵ���깤ʱ��
	vector<vector<int>> machinetime(ordernumber, vector<int>(parallel,0));//��¼��ǰ�׶ε�ǰ���л��Ŀ��п�ʼʱ��
	vector<vector<vector<int>>> starttime(workpiecesnumber, vector<vector<int>>(ordernumber, vector<int>(parallel, 0)));//ÿ����ҵ��ÿ���׶ε�ÿ̨�����ϵĿ�ʼʱ��
	vector<vector<vector<int>>> finishtime(workpiecesnumber, vector<vector<int>>(ordernumber, vector<int>(parallel, 0)));//ÿ����ҵ��ÿ���׶ε�ÿ̨�����ϵ����ʱ��

	for (int i = 0; i < ordernumber; i++)
	{
		for (int j = 0; j < workpiecesnumber; j++)
		{
			//Ѱ�ҵ�ǰ�׶�������еĻ������Ա�����ȼ���ߵ���ҵ���мӹ�
			int m = machinetime[i][0];
			int n = 0;
			for (int p = 0; p < parallel; p++)
			{
				if (m > machinetime[i][p])
				{
					m = machinetime[i][p];
					n = p;
				}
			}

			//��ѡ���ȼ���ߵ���ҵ���мӹ����������ʱ��
			int top = temp1[j];                 
			starttime[top][i][n] = max(machinetime[i][n], temp3[j]);  //��ʼ�ӹ�ʱ��=max���û����Ŀ��п�ʼʱ�䣬�ù�����һ�������깤ʱ�䣩
			machinetime[i][n] = starttime[top][i][n] + usetime[top][i]; //�������깤ʱ��=��ʼʱ��+�ӹ�ʱ��
			finishtime[top][i][n] = machinetime[i][n];                 
			temp2[j] = finishtime[top][i][n];
		}

		//�����깤ʱ���Ⱥ�����
		PDDE::BubbleSort(temp2, temp1);

		//������һ�׶���ҵ���깤ʱ�䣬������һ�׶μӹ�
		temp3 = temp2;
	}

	//����ÿ����ҵ�뿪���һ�������ʱ��
	vector<int> lasttime;
	for (int i = 0; i < workpiecesnumber; i++)
	{
		vector <int>::iterator index = find(temp1.begin(), temp1.end(), i);
		lasttime.push_back(temp2[distance(temp1.begin(), index)]);
	}

	//����������
	int makespan =0;
    for (int i=0; i<parallel; i++)
	{ 
		if (makespan < machinetime[ordernumber - 1][i])
			makespan = machinetime[ordernumber - 1][i];
	}

	//���������Ϣ
	person.starttime.push_back(starttime);
	person.finishtime.push_back(finishtime);
	person.lasttime.push_back(lasttime);
	person.makespan.push_back(makespan);
}

/********************
���ܣ�ð������
���룺
x ����������
y ���������������
*********************/
void PDDE::BubbleSort(vector<int>& x, vector<int>& y)
{
	for (int i = 0; i < x.size(); i++)
	{
		for (int j = 0; j < x.size() - i - 1; j++)
		{
			if (x[j] > x[j + 1])
			{
				int temp = x[j];
				x[j] = x[j + 1];
				x[j + 1] = temp;

				int ind_temp = y[j];
				y[j] = y[j + 1];
				y[j + 1] = ind_temp;
			}
		}
	}
}

/********************
���ܣ��������깤�ȼӹ�ԭ�򣬵õ�ÿ����ҵ��ÿ���׶ε�ÿ̨�����ϵĿ�ʼ����ɣ�ʱ�䣬�Լ�������
���룺
person ����
demand ÿ�ֲ�Ʒ����c��������
lasttime ÿ����ҵ���뿪���һ�������ʱ��
********************/
void PDDE::Total_Cost(Individual& person,vector<int> demand, vector<int> lasttime)
{
	int workpiecesnumber = person.sequence.size();//��ҵ��
	PDDE::product_types_to_jobs[PDDE::product_types - 1] = workpiecesnumber-accumulate(PDDE::product_types_to_jobs.begin(), PDDE::product_types_to_jobs.end() - 1, 0);

	double total_cost=0;
	for (int i = 0; i < PDDE::product_types; i++)
	{
		//����һ��д̫���ˣ��ֿ�д
		total_cost = total_cost + PDDE::unit_processing_cost[i] * PDDE::product_types_to_jobs[i];//�����ɱ�
		total_cost = total_cost + PDDE::unit_holding_cost[i] * max(PDDE::product_types_to_jobs[i] - demand[i], 0);//���ɱ�
		total_cost = total_cost + PDDE::unit_external_production_cost[i] * max(demand[i]-PDDE::product_types_to_jobs[i], 0);//�ⲿ�����ɱ�
	}
	for (int i = 0; i < workpiecesnumber; i++)
		total_cost = total_cost + PDDE::unit_delayed_completion_cost[i] * max(lasttime[i]-PDDE::due_date[i], 0);//�����깤�ɱ�

	person.total_cost.push_back(total_cost);
}

PDDE::Individual PDDE::Mutation_Current_1(Individual person_current, vector<Individual> pop, double factor)
{
	//���ݱ��칫ʽ��ñ������
	PDDE::Individual vi;

	if ((rand() % 100) * 0.01 < factor)
	{
		//�ڸ�����Ⱥ������ѡ����������
		int NPnum = pop.size();
		int r1 = rand() % NPnum;
		int r2 = rand() % NPnum;

		vector<int> di = PDDE::MinusOperation(pop[r1].sequence, pop[r2].sequence);
		vi.sequence = PDDE::AddOperation(person_current.sequence, di);
	}
	else
	{
		random_shuffle(person_current.sequence.begin(), person_current.sequence.end());
		vi.sequence = person_current.sequence;
	}

	return vi;
}

PDDE::Individual PDDE::Mutation_Current_1_Neighborhood(int generation, Individual person_current, vector<Individual> pop, double factor, Individual best)
{
	//ǰ��������PDDE/current/1���ԣ��˺����������������
	PDDE::Individual vi;

	if (generation <= 10)
		vi=PDDE::Mutation_Current_1(person_current, pop, factor);
	else
	{
		if ((rand() % 100) * 0.01 < factor)
		{
			//�ڸ�����Ⱥ������ѡ����������
			int NPnum = pop.size();
			int r1 = rand() % NPnum;
			int r2 = rand() % NPnum;

			vector<int> di = PDDE::MinusOperation(pop[r1].sequence, pop[r2].sequence);
			vi.sequence = PDDE::AddOperation(person_current.sequence, di);
		}
		//������������
		else
		{

			if ((rand() % 100) * 0.01 < 0.5)
				vi.sequence = PDDE::Swap(best.sequence);
			else
				vi.sequence = PDDE::Insert(best.sequence);
		}
	}

	return vi;
}

vector<int> PDDE::MinusOperation(vector<int> x, vector<int> y)
{
	//ͳһ������볤��
	vector<int> yp = y;
	if(x.size()!=y.size())
		yp = Uniform_Length(x, y);

	//����
	vector<int> z;
	for (int i = 0; i < x.size(); i++)
	{
		vector <int>::iterator index = find(yp.begin(), yp.end(), x[i]);
		z.push_back(distance(yp.begin(), index));
	}
	return z;
}

vector<int> PDDE::AddOperation(vector<int> x, vector<int> y)
{
	//ͳһ������볤��
	vector<int> yp = y;
	if (x.size() != y.size())
		yp = Uniform_Length(x, y);

	//�ӷ�
	vector<int> z;
	for (int i = 0; i < x.size(); i++)
	{
		z.push_back(x[yp[i]]);
	}
	return z;
}

vector<int> PDDE::Uniform_Length(vector<int> x, vector<int> y)
{
	if (x.size() == y.size())
		return y;

	if (x.size() > y.size())
	{
		vector<int> z = y;
		for (int i = 0; i < x.size() - y.size(); i++)
			z.push_back(y.size()+i);
		return z;
	}

	if (x.size() < y.size())
	{
		vector<int> z;
		for (int i = 0; i < y.size(); i++)
		{
			if (y[i] < x.size())
				z.push_back(y[i]);
		}
		return z;
	}
}

vector<int> PDDE::Swap(vector<int> x)
{
	vector<int> xswap = x;
	int position_a = rand() % xswap.size();
	int position_b = rand() % xswap.size();
	while (position_a == position_b)
		position_b = rand() % xswap.size();
	
	swap(xswap[position_a], xswap[position_b]);

	return xswap;
}

vector<int> PDDE::Insert(vector<int> x)
{
	vector<int> xinsert = x;
	int position_a = rand() % xinsert.size();
	int position_b = rand() % xinsert.size();
	while (position_a == position_b)
		position_b = rand() % xinsert.size();

	if (position_a < position_b)
	{
		xinsert.erase(xinsert.begin() + position_b);
		xinsert.insert(xinsert.begin() + position_a, x[position_b]);
	}
	else
	{
		xinsert.erase(xinsert.begin() + position_b);
		xinsert.insert(xinsert.begin() + position_a - 1, x[position_b]);
	}

	return xinsert;
}

PDDE::Individual PDDE::CrossOver_One_Point(Individual person1, Individual person2, double factor)
{
	//ͳһ������볤��
	vector<int> person1p = person1.sequence;
	if (person1.sequence.size() != person2.sequence.size())
		person1p = Uniform_Length(person2.sequence, person1.sequence);

	//���ݽ��湫ʽ��ý������
	PDDE::Individual person3;
	if ((rand() % 100) * 0.01 < factor)
		person3.sequence = PDDE::Overlapping(person1p, person2.sequence);//xi*vi
	else
		person3.sequence = PDDE::Overlapping(person2.sequence, person1p);//vi*xi
		
	return person3;
}

vector<int> PDDE::Overlapping(vector<int> x, vector<int> y)
{
	//����
	vector<int> z = y;
	int Cr_index = rand() % x.size();
	for (int i = 0; i < Cr_index; i++)
		z[i] = x[i];
	
	//�޸��ظ�Ԫ��
	for (int i = 0; i < Cr_index; i++)
	{
		vector<int>::iterator result = find(z.begin(), z.end(), y[i]);
		if (result == z.end())
			z[PDDE::Find_Repeat_Index(z)] = y[i];
	}
	return z;
}

int PDDE::Find_Repeat_Index(vector<int> x)
{
	int index = 0;
	int flag = 0;//�������ѭ����־
	for (int i = 1; i < x.size(); i++)
	{
		for (int j = i - 1; j > -1; j--)
		{
			if (x[i] == x[j])
			{
				index = i;
				flag = 1;
				break;
			}
		}
		if (flag == 1)
		{
			break;
		}
	}
	return index;
}

PDDE::Individual PDDE::CrossOver_Uniform(Individual person1, Individual person2, double factor)
{
	//ͳһ������볤��
	vector<int> person1p = person1.sequence;
	if (person1.sequence.size() != person2.sequence.size())
		person1p = Uniform_Length(person2.sequence, person1.sequence);

	//����
	PDDE::Individual person3;
	person3.sequence= person2.sequence;
	for (int i = 0; i < person2.sequence.size(); i++)
	{
		if ((rand() % 100) * 0.01 < factor)
			person3.sequence[i] = person1p[i];
	}

	//�޸��ظ�Ԫ��
	for (int i = 0; i < person2.sequence.size(); i++)
	{
		vector<int>::iterator result1 = find(person3.sequence.begin(), person3.sequence.end(), person2.sequence[i]);
		if (result1 == person3.sequence.end())
			person3.sequence[PDDE::Find_Repeat_Index(person3.sequence)] = person2.sequence[i];
	}

	return person3;
}

PDDE::Individual PDDE::Selection(Individual person1, Individual person2)
{
	if (person2.objective < person1.objective)
		return person2;
	else
		return person1;
}

PDDE::Individual PDDE::Find_Best_Individual(vector<Individual> pop)
{
	Individual min = pop[0];
	for (int i = 1; i < pop.size(); i++)
	{
		if (pop[i].objective < min.objective)
			min = pop[i];
	}
	return min;
}

void PDDE::Output_Individual_Information(Individual person)
{
	std::cout << "���������:" << endl;
	std::cout << "��ҵ���У�" << endl;
	for (int i = 0; i < person.sequence.size(); i++)
		std::cout << person.sequence[i] << "--";
	std::cout << endl;
	std::cout << "Ŀ�꺯��ֵ:" << person.objective << endl;
	std::cout << "�����ڵ�����" << person.expect_makespan << endl;
	std::cout << "�ܳɱ�������" << person.expect_total_cost << endl;
	std::cout << endl;
}

double PDDE::Standard(double avg, vector<PDDE::Result> message)
{
	int n = message.size();
	double sum = 0;
	for (int i = 0; i < n; i++)
		sum = sum + pow(message[i].best.objective - avg, 2);
	
	return sqrt(sum/n);
}

void PDDE::EDD_LPT(vector<int> x, vector<int>& y)
{
	for (int i = 0; i < x.size(); i++)
	{
		for (int j = i+1; j <x.size(); j++)
		{
			if (x[j] <= x[i])
			{
				int temp = x[i];
				x[i] = x[j];
				x[j] = temp;

				int ind_temp = y[i];
				y[i] = y[j];
				y[j] = ind_temp;
			}
		}
	}
}

PDDE::Individual PDDE::Find_Neighborhood(int structure, vector<int> x)
{
	if (structure == 0)
	{
		vector<PDDE::Individual> neighborhood_pop;
		neighborhood_pop.clear();
		srand((unsigned)time(NULL));
		for (int i = 0; i < PDDE::NP; i++)
		{
			PDDE::Individual xi_init;
			xi_init.sequence = PDDE::Swap(x);
			PDDE::Decoding_Individual(xi_init);
			neighborhood_pop.push_back(xi_init);
		}
		return PDDE::Find_Best_Individual(neighborhood_pop);
	}
	else
	{
		vector<PDDE::Individual> neighborhood_pop;
		neighborhood_pop.clear();
		srand((unsigned)time(NULL));
		for (int i = 0; i < PDDE::NP; i++)
		{
			PDDE::Individual xi_init;
			xi_init.sequence = PDDE::Insert(x);
			PDDE::Decoding_Individual(xi_init);
			neighborhood_pop.push_back(xi_init);
		}
		return PDDE::Find_Best_Individual(neighborhood_pop);
	}
}

void PDDE::Call_Heuristic(int jobscase, int stagescase, int machinescase, Result& result)
{
	//��ȡ����
	PDDE::jobs = jobscase;
	PDDE::stages = stagescase;
	PDDE::machines = machinescase;
	PDDE::Read_Data();

	//���÷��ɹ����ȡ��ʼ��
	vector<int> assign_solution;
	for (int i = 0; i < PDDE::jobs; i++)
	{
		assign_solution.push_back(i);
	}
	vector<int> current_due_date{ &PDDE::due_date[0],&PDDE::due_date[0] + assign_solution.size() };
	PDDE::EDD_LPT(current_due_date, assign_solution);

	PDDE::Individual best;
	best.sequence = assign_solution;
	PDDE::Decoding_Individual(best);

	clock_t start, stop;//��ʱ��
	start = clock();//��ʼ��ʱ

	int i = 0;
	while (i < 2)
	{
		PDDE::Individual newbest = PDDE::Find_Neighborhood(i,best.sequence);

		if (newbest.objective < best.objective)
		{
			best = newbest;
			i = 0;
		}
		else
			i = i + 1;
	}

	stop = clock();//������ʱ
	double computingtime = (double)(stop - start) / CLOCKS_PER_SEC;//����ʱ��

	//������
	std::cout << "����ʱ��CPU seconds=" << computingtime << endl;
	PDDE::Output_Individual_Information(best);

	// ������
	result.computingtime = computingtime;
	result.generation = 0;
	result.best = best;
}

void PDDE::Call_PDDE(int jobscase, int stagescase, int machinescase, Result& result)
{
	//��ȡ����
	PDDE::jobs = jobscase;
	PDDE::stages = stagescase;
	PDDE::machines = machinescase;
	PDDE::Read_Data();

	PDDE::Initial_Population(PDDE::initialpop, PDDE::jobs, PDDE::NP);//��ʼ����Ⱥ
	PDDE::Individual best = PDDE::Find_Best_Individual(PDDE::initialpop);//��¼���Ÿ���

	clock_t start, stop;//��ʱ��
	start = clock();//��ʼ��ʱ

	int generation = 0;//��¼ʵ�ʵ�������
	int reinit = 0;//��Ⱥ���Ÿ��峬��?��û�и�����ֹͣ��������¼û�и��µĵ�������
	while (generation < PDDE::Gmax && reinit < 10) //������ֹ����
	{
		generation++;

		vector<PDDE::Individual> newpop(PDDE::initialpop.size());
		srand((unsigned)time(NULL));
		for (int i = 0; i < PDDE::initialpop.size(); i++)
		{
			PDDE::Individual xi = PDDE::initialpop[i];//��ǰ����
			PDDE::Individual vi = PDDE::Mutation_Current_1(xi, PDDE::initialpop, PDDE::F);//��ñ������
			PDDE::Individual ui = PDDE::CrossOver_One_Point(xi, vi, PDDE::CR);//��ý������
			PDDE::Decoding_Individual(ui);//�����¸���
			PDDE::Individual newxi = PDDE::Selection(xi, ui);//ѡ��Ϻõĸ��������һ����Ⱥ
			newpop[i] = newxi;
		}
		PDDE::initialpop = newpop;

		PDDE::Individual newbest = PDDE::Find_Best_Individual(PDDE::initialpop);//Ѱ������Ⱥ���Ÿ���
		if (newbest.objective < best.objective)
		{
			best = newbest;
			reinit = 0;
		}
		else
			reinit++;
	}

	stop = clock();//������ʱ
	double computingtime = (double)(stop - start) / CLOCKS_PER_SEC;//����ʱ��

	//������
	std::cout << "����ʱ��CPU seconds=" << computingtime << endl;
	std::cout << "ʵ�ʵ�������Ϊ��" << generation << endl;
	PDDE::Output_Individual_Information(best);

	// ������
	result.computingtime=computingtime;
	result.generation=generation;
	result.best=best;
}

void PDDE::Call_PDDE_Variable(int jobscase, int stagescase, int machinescase, Result& result)
{
	//��ȡ����
	PDDE::jobs = jobscase + PDDE::Demand_Upper;
	PDDE::stages = stagescase;
	PDDE::machines = machinescase;
	PDDE::Read_Data();

	PDDE::Initial_Population_Variable(PDDE::initialpop, jobscase, PDDE::Demand_Lower, PDDE::Demand_Upper, PDDE::NP);//��ʼ����Ⱥ
	PDDE::Individual best = PDDE::Find_Best_Individual(PDDE::initialpop);//��¼���Ÿ���
	vector<double> iteration_value;//��¼ÿ�ε�����Ŀ�꺯��ֵ
	iteration_value.push_back(best.objective);

	clock_t start, stop;//��ʱ��
	double computingtime = 0;//��¼ʱ��
	start = clock();//��ʼ��ʱ

	int generation = 0;//��¼ʵ�ʵ�������
	while (computingtime < PDDE::TP * PDDE::jobs * PDDE::stages) //������ֹ����
	{
		generation++;

		vector<PDDE::Individual> newpop(PDDE::initialpop.size());
		srand((unsigned)time(NULL));
		for (int i = 0; i < PDDE::initialpop.size(); i++)
		{
			PDDE::Individual xi = PDDE::initialpop[i];//��ǰ����
			PDDE::Individual vi = PDDE::Mutation_Current_1(xi, PDDE::initialpop, PDDE::F);//��ñ�����壬PDDE/current/1
			PDDE::Individual ui = PDDE::CrossOver_One_Point(xi, vi, PDDE::CR);//��ý������,���㽻��
			//PDDE::Individual ui = PDDE::CrossOver_Uniform(xi, vi, PDDE::CR);//��ý������,���Ƚ���
			PDDE::Decoding_Individual(ui);//�����¸���
			PDDE::Individual newxi = PDDE::Selection(xi, ui);//ѡ��Ϻõĸ��������һ����Ⱥ
			newpop[i] = newxi;
		}
		PDDE::initialpop = newpop;

		PDDE::Individual newbest = PDDE::Find_Best_Individual(PDDE::initialpop);//Ѱ������Ⱥ���Ÿ���
		if (newbest.objective < best.objective)
			best = newbest;
		iteration_value.push_back(best.objective);

		stop = clock();//������ʱ
		computingtime = (double)(stop - start) / CLOCKS_PER_SEC;//����ʱ��
	}

	//������
	std::cout << "����ʱ��CPU seconds=" << computingtime << endl;
	std::cout << "ʵ�ʵ�������Ϊ��" << generation << endl;
	PDDE::Output_Individual_Information(best);

	// ������
	result.computingtime = computingtime;
	result.generation = generation;
	result.best = best;
	result.iteration_value = iteration_value;
}

void PDDE::Call_PDDE_Variable_Neighborhood(int jobscase, int stagescase, int machinescase, Result& result)
{
	//��ȡ����
	PDDE::jobs = jobscase + PDDE::Demand_Upper;
	PDDE::stages = stagescase;
	PDDE::machines = machinescase;
	PDDE::Read_Data();

	PDDE::Initial_Population_Variable(PDDE::initialpop, jobscase, PDDE::Demand_Lower, PDDE::Demand_Upper, PDDE::NP);//��ʼ����Ⱥ
	PDDE::Individual best = PDDE::Find_Best_Individual(PDDE::initialpop);//��¼���Ÿ���
	vector<double> iteration_value;//��¼ÿ�ε�����Ŀ�꺯��ֵ
	iteration_value.push_back(best.objective);

	clock_t start, stop;//��ʱ��
	double computingtime = 0;//��¼ʱ��
	start = clock();//��ʼ��ʱ

	int generation = 0;//��¼ʵ�ʵ�������
	while (computingtime < PDDE::TP * PDDE::jobs * PDDE::stages) //������ֹ����
	{
		generation++;

		srand((unsigned)time(NULL));
		for (int i = 0; i < PDDE::initialpop.size(); i++)
		{
			PDDE::Individual xi = PDDE::initialpop[i];//��ǰ����
			PDDE::Individual vi = PDDE::Mutation_Current_1_Neighborhood(generation, xi, PDDE::initialpop, PDDE::F, best);//��ñ�����壬PDDE/current/1+������������
			PDDE::Individual ui = PDDE::CrossOver_One_Point(xi, vi, PDDE::CR);//��ý������,���㽻��
			//PDDE::Individual ui = PDDE::CrossOver_Uniform(xi, vi, PDDE::CR);//��ý������,���Ƚ���
			PDDE::Decoding_Individual(ui);//�����¸���
			PDDE::Individual newxi = PDDE::Selection(xi, ui);//ѡ��Ϻõĸ��������һ����Ⱥ
			PDDE::initialpop[i] = newxi;
		}

		PDDE::Individual newbest = PDDE::Find_Best_Individual(PDDE::initialpop);//Ѱ������Ⱥ���Ÿ���
		if (newbest.objective < best.objective)
			best = newbest;
		iteration_value.push_back(best.objective);

		stop = clock();//������ʱ
		computingtime = (double)(stop - start) / CLOCKS_PER_SEC;//����ʱ��
	}

	//������
	std::cout << "����ʱ��CPU seconds=" << computingtime << endl;
	std::cout << "ʵ�ʵ�������Ϊ��" << generation << endl;
	PDDE::Output_Individual_Information(best);

	// ������
	result.computingtime = computingtime;
	result.generation = generation;
	result.best = best;
	result.iteration_value = iteration_value;
}

void PDDE::Call_PDDE_Variable_Heuristic(int jobscase, int stagescase, int machinescase, Result& result)
{
	//��ȡ����
	PDDE::jobs = jobscase + PDDE::Demand_Upper;
	PDDE::stages = stagescase;
	PDDE::machines = machinescase;
	PDDE::Read_Data();

	PDDE::Initial_Population_Variable_Heuristic(PDDE::initialpop, jobscase, PDDE::Demand_Lower, PDDE::Demand_Upper, PDDE::NP);//��ʼ����Ⱥ
	PDDE::Individual best = PDDE::Find_Best_Individual(PDDE::initialpop);//��¼���Ÿ���
	vector<double> iteration_value;//��¼ÿ�ε�����Ŀ�꺯��ֵ
	iteration_value.push_back(best.objective);

	clock_t start, stop;//��ʱ��
	double computingtime = 0;//��¼ʱ��
	start = clock();//��ʼ��ʱ

	int generation = 0;//��¼ʵ�ʵ�������
	while (computingtime < PDDE::TP * PDDE::jobs * PDDE::stages) //������ֹ����
	{
		generation++;

		vector<PDDE::Individual> newpop(PDDE::initialpop.size());
		srand((unsigned)time(NULL));
		for (int i = 0; i < PDDE::initialpop.size(); i++)
		{
			PDDE::Individual xi = PDDE::initialpop[i];//��ǰ����
			PDDE::Individual vi = PDDE::Mutation_Current_1(xi, PDDE::initialpop, PDDE::F);//��ñ�����壬PDDE/current/1
			PDDE::Individual ui = PDDE::CrossOver_One_Point(xi, vi, PDDE::CR);//��ý������,���㽻��
			//PDDE::Individual ui = PDDE::CrossOver_Uniform(xi, vi, PDDE::CR);//��ý������,���Ƚ���
			PDDE::Decoding_Individual(ui);//�����¸���
			PDDE::Individual newxi = PDDE::Selection(xi, ui);//ѡ��Ϻõĸ��������һ����Ⱥ
			newpop[i] = newxi;
		}
		PDDE::initialpop = newpop;

		PDDE::Individual newbest = PDDE::Find_Best_Individual(PDDE::initialpop);//Ѱ������Ⱥ���Ÿ���
		if (newbest.objective < best.objective)
			best = newbest;
		iteration_value.push_back(best.objective);

		stop = clock();//������ʱ
		computingtime = (double)(stop - start) / CLOCKS_PER_SEC;//����ʱ��
	}

	//������
	std::cout << "����ʱ��CPU seconds=" << computingtime << endl;
	std::cout << "ʵ�ʵ�������Ϊ��" << generation << endl;
	PDDE::Output_Individual_Information(best);

	// ������
	result.computingtime = computingtime;
	result.generation = generation;
	result.best = best;
	result.iteration_value = iteration_value;
}

void PDDE::Call_PDDE_Variable_Heuristic_Neighborhood(int jobscase, int stagescase, int machinescase, Result& result)
{
	//��ȡ����
	PDDE::jobs = jobscase + PDDE::Demand_Upper;
	PDDE::stages = stagescase;
	PDDE::machines = machinescase;
	PDDE::Read_Data();

	PDDE::Initial_Population_Variable_Heuristic(PDDE::initialpop, jobscase, PDDE::Demand_Lower, PDDE::Demand_Upper, PDDE::NP);//��ʼ����Ⱥ
	PDDE::Individual best = PDDE::Find_Best_Individual(PDDE::initialpop);//��¼���Ÿ���
	vector<double> iteration_value;//��¼ÿ�ε�����Ŀ�꺯��ֵ
	iteration_value.push_back(best.objective);

	clock_t start, stop;//��ʱ��
	double computingtime=0;//��¼ʱ��
	start = clock();//��ʼ��ʱ

	int generation = 0;//��¼ʵ�ʵ�������
	while (computingtime < PDDE::TP *PDDE::jobs* PDDE::stages) //������ֹ����
	{
		generation++;

		srand((unsigned)time(NULL));
		for (int i = 0; i < PDDE::initialpop.size(); i++)
		{
			PDDE::Individual xi = PDDE::initialpop[i];//��ǰ����
			PDDE::Individual vi = PDDE::Mutation_Current_1_Neighborhood(generation, xi, PDDE::initialpop, PDDE::F, best);//��ñ�����壬PDDE/current/1+������������
			PDDE::Individual ui = PDDE::CrossOver_One_Point(xi, vi, PDDE::CR);//��ý������,���㽻��
			//PDDE::Individual ui = PDDE::CrossOver_Uniform(xi, vi, PDDE::CR);//��ý������,���Ƚ���
			PDDE::Decoding_Individual(ui);//�����¸���
			PDDE::Individual newxi = PDDE::Selection(xi, ui);//ѡ��Ϻõĸ��������һ����Ⱥ
			PDDE::initialpop[i] = newxi;
		}

		PDDE::Individual newbest = PDDE::Find_Best_Individual(PDDE::initialpop);//Ѱ������Ⱥ���Ÿ���
		if (newbest.objective < best.objective)
			best = newbest;
		iteration_value.push_back(best.objective);

		stop = clock();//������ʱ
		computingtime = (double)(stop - start) / CLOCKS_PER_SEC;//����ʱ��
	}

	//������
	std::cout << "����ʱ��CPU seconds=" << computingtime << endl;
	std::cout << "ʵ�ʵ�������Ϊ��" << generation << endl;
	PDDE::Output_Individual_Information(best);

	// ������
	result.computingtime = computingtime;
	result.generation = generation;
	result.best = best;
	result.iteration_value = iteration_value;
}

void PDDE::Call_PDDE_Variable_Heuristic_Neighborhood_determine_demand(int jobscase, int stagescase, int machinescase, int avgdemand, Result& result)
{
	//��ȡ����
	PDDE::jobs = jobscase + PDDE::Demand_Upper;
	PDDE::stages = stagescase;
	PDDE::machines = machinescase;
	PDDE::Read_Data();

	//����ȷ�������ر��ȷ����
	for (int i = 0; i < PDDE::scenarios; i++)
		PDDE::Demand[i][PDDE::product_types - 1] = avgdemand;

	PDDE::Initial_Population_Variable_Heuristic(PDDE::initialpop, jobscase, PDDE::Demand_Lower, PDDE::Demand_Upper, PDDE::NP);//��ʼ����Ⱥ
	PDDE::Individual best = PDDE::Find_Best_Individual(PDDE::initialpop);//��¼���Ÿ���

	clock_t start, stop;//��ʱ��
	double computingtime = 0;//��¼ʱ��
	start = clock();//��ʼ��ʱ

	int generation = 0;//��¼ʵ�ʵ�������
	while (computingtime < PDDE::TP * PDDE::jobs * PDDE::stages) //������ֹ����
	{
		generation++;

		srand((unsigned)time(NULL));
		for (int i = 0; i < PDDE::initialpop.size(); i++)
		{
			PDDE::Individual xi = PDDE::initialpop[i];//��ǰ����
			PDDE::Individual vi = PDDE::Mutation_Current_1_Neighborhood(generation, xi, PDDE::initialpop, PDDE::F, best);//��ñ�����壬PDDE/current/1+������������
			PDDE::Individual ui = PDDE::CrossOver_One_Point(xi, vi, PDDE::CR);//��ý������,���㽻��
			//PDDE::Individual ui = PDDE::CrossOver_Uniform(xi, vi, PDDE::CR);//��ý������,���Ƚ���
			PDDE::Decoding_Individual(ui);//�����¸���
			PDDE::Individual newxi = PDDE::Selection(xi, ui);//ѡ��Ϻõĸ��������һ����Ⱥ
			PDDE::initialpop[i] = newxi;
		}

		PDDE::Individual newbest = PDDE::Find_Best_Individual(PDDE::initialpop);//Ѱ������Ⱥ���Ÿ���
		if (newbest.objective < best.objective)
			best = newbest;
		
		stop = clock();//������ʱ
		computingtime = (double)(stop - start) / CLOCKS_PER_SEC;//����ʱ��

	}

	//������
	PDDE::Output_Individual_Information(best);

	// ������
	result.best = best;
}

void PDDE::Call_PDDE_Variable_Heuristic_Neighborhood_determine_processtime(int jobscase, int stagescase, int machinescase, int avgprocesstime, Result& result)
{
	//��ȡ����
	PDDE::jobs = jobscase + PDDE::Demand_Upper;
	PDDE::stages = stagescase;
	PDDE::machines = machinescase;
	PDDE::Read_Data();

	//����ȷ�������ر��ȷ����
	for (int n = 0; n < PDDE::scenarios; n++)
	{
		int jobindex = accumulate(PDDE::product_types_to_jobs.begin(), PDDE::product_types_to_jobs.end() - 1, 0);
		for (int y = 0; y < PDDE::product_types_to_jobs[PDDE::product_types - 1]; y++)
		{
			PDDE::processing_time[n][jobindex][PDDE::stages - 1] = avgprocesstime;
			jobindex++;
		}
	}

	PDDE::Initial_Population_Variable_Heuristic(PDDE::initialpop, jobscase, PDDE::Demand_Lower, PDDE::Demand_Upper, PDDE::NP);//��ʼ����Ⱥ
	PDDE::Individual best = PDDE::Find_Best_Individual(PDDE::initialpop);//��¼���Ÿ���

	clock_t start, stop;//��ʱ��
	double computingtime = 0;//��¼ʱ��
	start = clock();//��ʼ��ʱ

	int generation = 0;//��¼ʵ�ʵ�������
	while (computingtime < PDDE::TP * PDDE::jobs * PDDE::stages) //������ֹ����
	{
		generation++;

		srand((unsigned)time(NULL));
		for (int i = 0; i < PDDE::initialpop.size(); i++)
		{
			PDDE::Individual xi = PDDE::initialpop[i];//��ǰ����
			PDDE::Individual vi = PDDE::Mutation_Current_1_Neighborhood(generation, xi, PDDE::initialpop, PDDE::F, best);//��ñ�����壬PDDE/current/1+������������
			PDDE::Individual ui = PDDE::CrossOver_One_Point(xi, vi, PDDE::CR);//��ý������,���㽻��
			//PDDE::Individual ui = PDDE::CrossOver_Uniform(xi, vi, PDDE::CR);//��ý������,���Ƚ���
			PDDE::Decoding_Individual(ui);//�����¸���
			PDDE::Individual newxi = PDDE::Selection(xi, ui);//ѡ��Ϻõĸ��������һ����Ⱥ
			PDDE::initialpop[i] = newxi;
		}

		PDDE::Individual newbest = PDDE::Find_Best_Individual(PDDE::initialpop);//Ѱ������Ⱥ���Ÿ���
		if (newbest.objective < best.objective)
			best = newbest;

		stop = clock();//������ʱ
		computingtime = (double)(stop - start) / CLOCKS_PER_SEC;//����ʱ��

	}

	//������
	PDDE::Output_Individual_Information(best);

	// ������
	result.best = best;
}