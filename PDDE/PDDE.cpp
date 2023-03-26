//PDDE.cpp：PDDE算法的类

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
	
	//小规模实例的数据
	PDDE::scenarios = 3;
	//PDDE::Demand_Lower = 0;
	//PDDE::Demand_Upper = 0;
	//PDDE::jobs = 10;//取{10,15}
	//PDDE::stages = 2;//取{2,5}
	//PDDE::machines = 1;//取{1,2,3}
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

	//第一种产品类型在阶段k的加工时间
	vector<int> processing_time_product_types_top;
	processing_time_product_types_top.clear();
	processing_time_product_types_top.push_back(1);
	processing_time_product_types_top.push_back(2);
	processing_time_product_types_top.push_back(3);
	processing_time_product_types_top.push_back(3);
	processing_time_product_types_top.push_back(2);
	//产品类型c在阶段k的加工时间
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
	//场景s下最后一种产品类型c在最后一个阶段k的加工时间
	PDDE::processing_time_uncertain.clear();
	PDDE::processing_time_uncertain.push_back(3);
	PDDE::processing_time_uncertain.push_back(4);
	PDDE::processing_time_uncertain.push_back(5);
	//场景s下产品类型c在阶段k的加工时间
	vector<vector<vector<int>>> processing_time_product_types_scenarios;
	processing_time_product_types_scenarios.clear();
	for (int i = 0; i < PDDE::scenarios; i++)
	{
		processing_time_product_types[PDDE::product_types - 1][PDDE::stages - 1] = PDDE::processing_time_uncertain[i];
		processing_time_product_types_scenarios.push_back(processing_time_product_types);
	}
	//场景s下作业j在阶段k的加工时间
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
	
	//产品类型c的需求量
	vector<int> Demand_product_types = PDDE::product_types_to_jobs;
	//场景s下产品类型c的需求量
	PDDE::Demand.clear();
	for (int i = 0; i < PDDE::scenarios; i++)
		PDDE::Demand.push_back(Demand_product_types);

	//产品类型c的到期日
	vector<int> due_date_product_types;
	due_date_product_types.clear();
	due_date_product_types.push_back(10);
	due_date_product_types.push_back(20);
	due_date_product_types.push_back(30);
	//作业j的到期日，产品类型相同的作业到期日也相同
	//PDDE::due_date.clear();
	//for (int i = 0; i < PDDE::product_types; i++)
	//{
	//	for (int j = 0; j < PDDE::product_types_to_jobs[i]; j++)
	//		PDDE::due_date.push_back(due_date_product_types[i]);
	//}
	//作业j的到期日，任意一个例子
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

	//产品类型c的单位延期完工成本
	vector<double> unit_delayed_completion_product_types;
	unit_delayed_completion_product_types.clear();
	unit_delayed_completion_product_types.push_back(1);
	unit_delayed_completion_product_types.push_back(2);
	unit_delayed_completion_product_types.push_back(3);
	//作业j的单位延期完工成本
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
	//大规模实例的数据
	PDDE::scenarios = 5;
	//PDDE::Demand_Lower = 5;
	//PDDE::Demand_Upper = 9;
	//PDDE::jobs = 20;//取{20,50,100}
	//PDDE::stages = 2;//取{2,5,8}
	//PDDE::machines = 3;//取{3,4}
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
	
	//第一种产品类型在阶段k的加工时间
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
	//产品类型c在阶段k的加工时间
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
	//场景s下产品类型c在阶段k的加工时间
	vector<vector<vector<int>>> processing_time_product_types_scenarios;
	processing_time_product_types_scenarios.clear();
	for (int i = 0; i < PDDE::scenarios; i++)
		processing_time_product_types_scenarios.push_back(processing_time_product_types);
	//场景s下作业j在阶段k的加工时间
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

	//场景s下最后一种产品类型的需求量
	PDDE::Demand_uncertain.clear();
	PDDE::Demand_uncertain.push_back(5);
	PDDE::Demand_uncertain.push_back(6);
	PDDE::Demand_uncertain.push_back(7);
	PDDE::Demand_uncertain.push_back(8);
	PDDE::Demand_uncertain.push_back(9);
	//产品类型c的需求量
	vector<int> Demand_product_types = PDDE::product_types_to_jobs;
	//场景s下产品类型c的需求量
	PDDE::Demand.clear();
	for (int i = 0; i < PDDE::scenarios; i++)
	{
		Demand_product_types[PDDE::product_types - 1] = PDDE::Demand_uncertain[i];
		PDDE::Demand.push_back(Demand_product_types);

	}
	
	//产品类型c的到期日
	vector<int> due_date_product_types;
	due_date_product_types.clear();
	due_date_product_types.push_back(15);
	due_date_product_types.push_back(20);
	due_date_product_types.push_back(25);
	due_date_product_types.push_back(30);
	due_date_product_types.push_back(35);
	due_date_product_types.push_back(40);
	//作业j的到期日，产品类型相同的作业到期日也相同
	//PDDE::due_date.clear();
	//for (int i = 0; i < PDDE::product_types; i++)
	//{
	//	for (int j = 0; j < PDDE::product_types_to_jobs[i]; j++)
	//		PDDE::due_date.push_back(due_date_product_types[i]);
	//}
	//作业j的到期日，任意一个例子
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

	//产品类型c的单位延期完工成本
	vector<double> unit_delayed_completion_product_types;
	unit_delayed_completion_product_types.clear();
	unit_delayed_completion_product_types.push_back(1);
	unit_delayed_completion_product_types.push_back(1.5);
	unit_delayed_completion_product_types.push_back(2);
	unit_delayed_completion_product_types.push_back(2.5);
	unit_delayed_completion_product_types.push_back(3);
	unit_delayed_completion_product_types.push_back(3.5);
	//作业j的单位延期完工成本
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
	//初始化作业序列，为jobnum-1的连续整数序列
	vector<int> index;
	for (int i = 0; i < jobnum; i++)
	{
		index.push_back(i);
	}

	//随机打乱序列
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
	int sub_pop=Upper - Lower + 1;//分成Upper - Lower + 1个子种群
	int sub_pop_NP = round(NPnum / sub_pop);//每个子种群的数量

	//初始化作业序列
	vector<int> index;
	for (int i = 0; i < jobnum + Lower; i++)
	{
		index.push_back(i);
	}

	//随机打乱序列
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
	int sub_pop = Upper - Lower + 1;//分成Upper - Lower + 1个子种群
	int sub_pop_NP = round(NPnum / sub_pop);//每个子种群的数量

	//初始化作业序列
	vector<int> index;
	for (int i = 0; i < jobnum + Lower; i++)
	{
		index.push_back(i);
	}

	//随机打乱序列
	pop.clear();
	srand((unsigned)time(NULL));
	for (int i = 0; i < sub_pop; i++)
	{	
		//运用分派规则和VND结合的启发式算法获取初始解
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

		//扰动产生多种群
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
功能：根据先完工先加工原则，得到每项作业在每个阶段的每台机器上的开始（完成）时间，以及制造期
输入：
person 个体
ordernumber 阶段数
parallel 每个阶段的并行机个数
usetime 每个作业在每个阶段的加工时间
这段代码的变量名称是参照别人的代码
*********************/
void PDDE::Makespan(Individual& person,int ordernumber, int parallel, vector<vector<int>> usetime)
{
    int workpiecesnumber = person.sequence.size();//作业数
	vector<int> temp1 = person.sequence;//记录当前阶段作业的优先级，第一阶段的优先级为编码
	vector<int> temp2(workpiecesnumber,0);//记录当前阶段作业的完工时间，根据完工时间先后，确定下一阶段作业的优先级
	vector<int> temp3(workpiecesnumber,0);//记录上一阶段作业的完工时间
	vector<vector<int>> machinetime(ordernumber, vector<int>(parallel,0));//记录当前阶段当前并行机的空闲开始时间
	vector<vector<vector<int>>> starttime(workpiecesnumber, vector<vector<int>>(ordernumber, vector<int>(parallel, 0)));//每项作业在每个阶段的每台机器上的开始时间
	vector<vector<vector<int>>> finishtime(workpiecesnumber, vector<vector<int>>(ordernumber, vector<int>(parallel, 0)));//每项作业在每个阶段的每台机器上的完成时间

	for (int i = 0; i < ordernumber; i++)
	{
		for (int j = 0; j < workpiecesnumber; j++)
		{
			//寻找当前阶段最早空闲的机器，以便给优先级最高的作业进行加工
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

			//挑选优先级最高的作业进行加工，计算完成时间
			int top = temp1[j];                 
			starttime[top][i][n] = max(machinetime[i][n], temp3[j]);  //开始加工时间=max（该机器的空闲开始时间，该工件上一道工序完工时间）
			machinetime[i][n] = starttime[top][i][n] + usetime[top][i]; //工件的完工时间=开始时间+加工时间
			finishtime[top][i][n] = machinetime[i][n];                 
			temp2[j] = finishtime[top][i][n];
		}

		//根据完工时间先后排序
		PDDE::BubbleSort(temp2, temp1);

		//更新上一阶段作业的完工时间，进入下一阶段加工
		temp3 = temp2;
	}

	//计算每项作业离开最后一道工序的时间
	vector<int> lasttime;
	for (int i = 0; i < workpiecesnumber; i++)
	{
		vector <int>::iterator index = find(temp1.begin(), temp1.end(), i);
		lasttime.push_back(temp2[distance(temp1.begin(), index)]);
	}

	//计算制造期
	int makespan =0;
    for (int i=0; i<parallel; i++)
	{ 
		if (makespan < machinetime[ordernumber - 1][i])
			makespan = machinetime[ordernumber - 1][i];
	}

	//保存个体信息
	person.starttime.push_back(starttime);
	person.finishtime.push_back(finishtime);
	person.lasttime.push_back(lasttime);
	person.makespan.push_back(makespan);
}

/********************
功能：冒泡排序
输入：
x 待排序数组
y 待排序数组的索引
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
功能：根据先完工先加工原则，得到每项作业在每个阶段的每台机器上的开始（完成）时间，以及制造期
输入：
person 个体
demand 每种产品类型c的需求量
lasttime 每项作业的离开最后一道工序的时间
********************/
void PDDE::Total_Cost(Individual& person,vector<int> demand, vector<int> lasttime)
{
	int workpiecesnumber = person.sequence.size();//作业数
	PDDE::product_types_to_jobs[PDDE::product_types - 1] = workpiecesnumber-accumulate(PDDE::product_types_to_jobs.begin(), PDDE::product_types_to_jobs.end() - 1, 0);

	double total_cost=0;
	for (int i = 0; i < PDDE::product_types; i++)
	{
		//放在一起写太长了，分开写
		total_cost = total_cost + PDDE::unit_processing_cost[i] * PDDE::product_types_to_jobs[i];//生产成本
		total_cost = total_cost + PDDE::unit_holding_cost[i] * max(PDDE::product_types_to_jobs[i] - demand[i], 0);//库存成本
		total_cost = total_cost + PDDE::unit_external_production_cost[i] * max(demand[i]-PDDE::product_types_to_jobs[i], 0);//外部生产成本
	}
	for (int i = 0; i < workpiecesnumber; i++)
		total_cost = total_cost + PDDE::unit_delayed_completion_cost[i] * max(lasttime[i]-PDDE::due_date[i], 0);//延期完工成本

	person.total_cost.push_back(total_cost);
}

PDDE::Individual PDDE::Mutation_Current_1(Individual person_current, vector<Individual> pop, double factor)
{
	//根据变异公式获得变异个体
	PDDE::Individual vi;

	if ((rand() % 100) * 0.01 < factor)
	{
		//在父代种群中任意选择两个个体
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
	//前？代采用PDDE/current/1策略，此后加入邻域搜索策略
	PDDE::Individual vi;

	if (generation <= 10)
		vi=PDDE::Mutation_Current_1(person_current, pop, factor);
	else
	{
		if ((rand() % 100) * 0.01 < factor)
		{
			//在父代种群中任意选择两个个体
			int NPnum = pop.size();
			int r1 = rand() % NPnum;
			int r2 = rand() % NPnum;

			vector<int> di = PDDE::MinusOperation(pop[r1].sequence, pop[r2].sequence);
			vi.sequence = PDDE::AddOperation(person_current.sequence, di);
		}
		//邻域搜索策略
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
	//统一个体编码长度
	vector<int> yp = y;
	if(x.size()!=y.size())
		yp = Uniform_Length(x, y);

	//减法
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
	//统一个体编码长度
	vector<int> yp = y;
	if (x.size() != y.size())
		yp = Uniform_Length(x, y);

	//加法
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
	//统一个体编码长度
	vector<int> person1p = person1.sequence;
	if (person1.sequence.size() != person2.sequence.size())
		person1p = Uniform_Length(person2.sequence, person1.sequence);

	//根据交叉公式获得交叉个体
	PDDE::Individual person3;
	if ((rand() % 100) * 0.01 < factor)
		person3.sequence = PDDE::Overlapping(person1p, person2.sequence);//xi*vi
	else
		person3.sequence = PDDE::Overlapping(person2.sequence, person1p);//vi*xi
		
	return person3;
}

vector<int> PDDE::Overlapping(vector<int> x, vector<int> y)
{
	//交叉
	vector<int> z = y;
	int Cr_index = rand() % x.size();
	for (int i = 0; i < Cr_index; i++)
		z[i] = x[i];
	
	//修改重复元素
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
	int flag = 0;//跳出外层循环标志
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
	//统一个体编码长度
	vector<int> person1p = person1.sequence;
	if (person1.sequence.size() != person2.sequence.size())
		person1p = Uniform_Length(person2.sequence, person1.sequence);

	//交叉
	PDDE::Individual person3;
	person3.sequence= person2.sequence;
	for (int i = 0; i < person2.sequence.size(); i++)
	{
		if ((rand() % 100) * 0.01 < factor)
			person3.sequence[i] = person1p[i];
	}

	//修改重复元素
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
	std::cout << "输出个体结果:" << endl;
	std::cout << "作业序列：" << endl;
	for (int i = 0; i < person.sequence.size(); i++)
		std::cout << person.sequence[i] << "--";
	std::cout << endl;
	std::cout << "目标函数值:" << person.objective << endl;
	std::cout << "制造期的期望" << person.expect_makespan << endl;
	std::cout << "总成本的期望" << person.expect_total_cost << endl;
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
	//读取数据
	PDDE::jobs = jobscase;
	PDDE::stages = stagescase;
	PDDE::machines = machinescase;
	PDDE::Read_Data();

	//运用分派规则获取初始解
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

	clock_t start, stop;//计时器
	start = clock();//开始计时

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

	stop = clock();//结束计时
	double computingtime = (double)(stop - start) / CLOCKS_PER_SEC;//计算时间

	//输出结果
	std::cout << "计算时间CPU seconds=" << computingtime << endl;
	PDDE::Output_Individual_Information(best);

	// 保存结果
	result.computingtime = computingtime;
	result.generation = 0;
	result.best = best;
}

void PDDE::Call_PDDE(int jobscase, int stagescase, int machinescase, Result& result)
{
	//读取数据
	PDDE::jobs = jobscase;
	PDDE::stages = stagescase;
	PDDE::machines = machinescase;
	PDDE::Read_Data();

	PDDE::Initial_Population(PDDE::initialpop, PDDE::jobs, PDDE::NP);//初始化种群
	PDDE::Individual best = PDDE::Find_Best_Individual(PDDE::initialpop);//记录最优个体

	clock_t start, stop;//计时器
	start = clock();//开始计时

	int generation = 0;//记录实际迭代次数
	int reinit = 0;//种群最优个体超过?代没有更新则停止迭代，记录没有更新的迭代次数
	while (generation < PDDE::Gmax && reinit < 10) //迭代终止条件
	{
		generation++;

		vector<PDDE::Individual> newpop(PDDE::initialpop.size());
		srand((unsigned)time(NULL));
		for (int i = 0; i < PDDE::initialpop.size(); i++)
		{
			PDDE::Individual xi = PDDE::initialpop[i];//当前个体
			PDDE::Individual vi = PDDE::Mutation_Current_1(xi, PDDE::initialpop, PDDE::F);//获得变异个体
			PDDE::Individual ui = PDDE::CrossOver_One_Point(xi, vi, PDDE::CR);//获得交叉个体
			PDDE::Decoding_Individual(ui);//解码新个体
			PDDE::Individual newxi = PDDE::Selection(xi, ui);//选择较好的个体进入下一代种群
			newpop[i] = newxi;
		}
		PDDE::initialpop = newpop;

		PDDE::Individual newbest = PDDE::Find_Best_Individual(PDDE::initialpop);//寻找新种群最优个体
		if (newbest.objective < best.objective)
		{
			best = newbest;
			reinit = 0;
		}
		else
			reinit++;
	}

	stop = clock();//结束计时
	double computingtime = (double)(stop - start) / CLOCKS_PER_SEC;//计算时间

	//输出结果
	std::cout << "计算时间CPU seconds=" << computingtime << endl;
	std::cout << "实际迭代次数为：" << generation << endl;
	PDDE::Output_Individual_Information(best);

	// 保存结果
	result.computingtime=computingtime;
	result.generation=generation;
	result.best=best;
}

void PDDE::Call_PDDE_Variable(int jobscase, int stagescase, int machinescase, Result& result)
{
	//读取数据
	PDDE::jobs = jobscase + PDDE::Demand_Upper;
	PDDE::stages = stagescase;
	PDDE::machines = machinescase;
	PDDE::Read_Data();

	PDDE::Initial_Population_Variable(PDDE::initialpop, jobscase, PDDE::Demand_Lower, PDDE::Demand_Upper, PDDE::NP);//初始化种群
	PDDE::Individual best = PDDE::Find_Best_Individual(PDDE::initialpop);//记录最优个体
	vector<double> iteration_value;//记录每次迭代的目标函数值
	iteration_value.push_back(best.objective);

	clock_t start, stop;//计时器
	double computingtime = 0;//记录时间
	start = clock();//开始计时

	int generation = 0;//记录实际迭代次数
	while (computingtime < PDDE::TP * PDDE::jobs * PDDE::stages) //迭代终止条件
	{
		generation++;

		vector<PDDE::Individual> newpop(PDDE::initialpop.size());
		srand((unsigned)time(NULL));
		for (int i = 0; i < PDDE::initialpop.size(); i++)
		{
			PDDE::Individual xi = PDDE::initialpop[i];//当前个体
			PDDE::Individual vi = PDDE::Mutation_Current_1(xi, PDDE::initialpop, PDDE::F);//获得变异个体，PDDE/current/1
			PDDE::Individual ui = PDDE::CrossOver_One_Point(xi, vi, PDDE::CR);//获得交叉个体,单点交叉
			//PDDE::Individual ui = PDDE::CrossOver_Uniform(xi, vi, PDDE::CR);//获得交叉个体,均匀交叉
			PDDE::Decoding_Individual(ui);//解码新个体
			PDDE::Individual newxi = PDDE::Selection(xi, ui);//选择较好的个体进入下一代种群
			newpop[i] = newxi;
		}
		PDDE::initialpop = newpop;

		PDDE::Individual newbest = PDDE::Find_Best_Individual(PDDE::initialpop);//寻找新种群最优个体
		if (newbest.objective < best.objective)
			best = newbest;
		iteration_value.push_back(best.objective);

		stop = clock();//结束计时
		computingtime = (double)(stop - start) / CLOCKS_PER_SEC;//计算时间
	}

	//输出结果
	std::cout << "计算时间CPU seconds=" << computingtime << endl;
	std::cout << "实际迭代次数为：" << generation << endl;
	PDDE::Output_Individual_Information(best);

	// 保存结果
	result.computingtime = computingtime;
	result.generation = generation;
	result.best = best;
	result.iteration_value = iteration_value;
}

void PDDE::Call_PDDE_Variable_Neighborhood(int jobscase, int stagescase, int machinescase, Result& result)
{
	//读取数据
	PDDE::jobs = jobscase + PDDE::Demand_Upper;
	PDDE::stages = stagescase;
	PDDE::machines = machinescase;
	PDDE::Read_Data();

	PDDE::Initial_Population_Variable(PDDE::initialpop, jobscase, PDDE::Demand_Lower, PDDE::Demand_Upper, PDDE::NP);//初始化种群
	PDDE::Individual best = PDDE::Find_Best_Individual(PDDE::initialpop);//记录最优个体
	vector<double> iteration_value;//记录每次迭代的目标函数值
	iteration_value.push_back(best.objective);

	clock_t start, stop;//计时器
	double computingtime = 0;//记录时间
	start = clock();//开始计时

	int generation = 0;//记录实际迭代次数
	while (computingtime < PDDE::TP * PDDE::jobs * PDDE::stages) //迭代终止条件
	{
		generation++;

		srand((unsigned)time(NULL));
		for (int i = 0; i < PDDE::initialpop.size(); i++)
		{
			PDDE::Individual xi = PDDE::initialpop[i];//当前个体
			PDDE::Individual vi = PDDE::Mutation_Current_1_Neighborhood(generation, xi, PDDE::initialpop, PDDE::F, best);//获得变异个体，PDDE/current/1+邻域搜索策略
			PDDE::Individual ui = PDDE::CrossOver_One_Point(xi, vi, PDDE::CR);//获得交叉个体,单点交叉
			//PDDE::Individual ui = PDDE::CrossOver_Uniform(xi, vi, PDDE::CR);//获得交叉个体,均匀交叉
			PDDE::Decoding_Individual(ui);//解码新个体
			PDDE::Individual newxi = PDDE::Selection(xi, ui);//选择较好的个体进入下一代种群
			PDDE::initialpop[i] = newxi;
		}

		PDDE::Individual newbest = PDDE::Find_Best_Individual(PDDE::initialpop);//寻找新种群最优个体
		if (newbest.objective < best.objective)
			best = newbest;
		iteration_value.push_back(best.objective);

		stop = clock();//结束计时
		computingtime = (double)(stop - start) / CLOCKS_PER_SEC;//计算时间
	}

	//输出结果
	std::cout << "计算时间CPU seconds=" << computingtime << endl;
	std::cout << "实际迭代次数为：" << generation << endl;
	PDDE::Output_Individual_Information(best);

	// 保存结果
	result.computingtime = computingtime;
	result.generation = generation;
	result.best = best;
	result.iteration_value = iteration_value;
}

void PDDE::Call_PDDE_Variable_Heuristic(int jobscase, int stagescase, int machinescase, Result& result)
{
	//读取数据
	PDDE::jobs = jobscase + PDDE::Demand_Upper;
	PDDE::stages = stagescase;
	PDDE::machines = machinescase;
	PDDE::Read_Data();

	PDDE::Initial_Population_Variable_Heuristic(PDDE::initialpop, jobscase, PDDE::Demand_Lower, PDDE::Demand_Upper, PDDE::NP);//初始化种群
	PDDE::Individual best = PDDE::Find_Best_Individual(PDDE::initialpop);//记录最优个体
	vector<double> iteration_value;//记录每次迭代的目标函数值
	iteration_value.push_back(best.objective);

	clock_t start, stop;//计时器
	double computingtime = 0;//记录时间
	start = clock();//开始计时

	int generation = 0;//记录实际迭代次数
	while (computingtime < PDDE::TP * PDDE::jobs * PDDE::stages) //迭代终止条件
	{
		generation++;

		vector<PDDE::Individual> newpop(PDDE::initialpop.size());
		srand((unsigned)time(NULL));
		for (int i = 0; i < PDDE::initialpop.size(); i++)
		{
			PDDE::Individual xi = PDDE::initialpop[i];//当前个体
			PDDE::Individual vi = PDDE::Mutation_Current_1(xi, PDDE::initialpop, PDDE::F);//获得变异个体，PDDE/current/1
			PDDE::Individual ui = PDDE::CrossOver_One_Point(xi, vi, PDDE::CR);//获得交叉个体,单点交叉
			//PDDE::Individual ui = PDDE::CrossOver_Uniform(xi, vi, PDDE::CR);//获得交叉个体,均匀交叉
			PDDE::Decoding_Individual(ui);//解码新个体
			PDDE::Individual newxi = PDDE::Selection(xi, ui);//选择较好的个体进入下一代种群
			newpop[i] = newxi;
		}
		PDDE::initialpop = newpop;

		PDDE::Individual newbest = PDDE::Find_Best_Individual(PDDE::initialpop);//寻找新种群最优个体
		if (newbest.objective < best.objective)
			best = newbest;
		iteration_value.push_back(best.objective);

		stop = clock();//结束计时
		computingtime = (double)(stop - start) / CLOCKS_PER_SEC;//计算时间
	}

	//输出结果
	std::cout << "计算时间CPU seconds=" << computingtime << endl;
	std::cout << "实际迭代次数为：" << generation << endl;
	PDDE::Output_Individual_Information(best);

	// 保存结果
	result.computingtime = computingtime;
	result.generation = generation;
	result.best = best;
	result.iteration_value = iteration_value;
}

void PDDE::Call_PDDE_Variable_Heuristic_Neighborhood(int jobscase, int stagescase, int machinescase, Result& result)
{
	//读取数据
	PDDE::jobs = jobscase + PDDE::Demand_Upper;
	PDDE::stages = stagescase;
	PDDE::machines = machinescase;
	PDDE::Read_Data();

	PDDE::Initial_Population_Variable_Heuristic(PDDE::initialpop, jobscase, PDDE::Demand_Lower, PDDE::Demand_Upper, PDDE::NP);//初始化种群
	PDDE::Individual best = PDDE::Find_Best_Individual(PDDE::initialpop);//记录最优个体
	vector<double> iteration_value;//记录每次迭代的目标函数值
	iteration_value.push_back(best.objective);

	clock_t start, stop;//计时器
	double computingtime=0;//记录时间
	start = clock();//开始计时

	int generation = 0;//记录实际迭代次数
	while (computingtime < PDDE::TP *PDDE::jobs* PDDE::stages) //迭代终止条件
	{
		generation++;

		srand((unsigned)time(NULL));
		for (int i = 0; i < PDDE::initialpop.size(); i++)
		{
			PDDE::Individual xi = PDDE::initialpop[i];//当前个体
			PDDE::Individual vi = PDDE::Mutation_Current_1_Neighborhood(generation, xi, PDDE::initialpop, PDDE::F, best);//获得变异个体，PDDE/current/1+邻域搜索策略
			PDDE::Individual ui = PDDE::CrossOver_One_Point(xi, vi, PDDE::CR);//获得交叉个体,单点交叉
			//PDDE::Individual ui = PDDE::CrossOver_Uniform(xi, vi, PDDE::CR);//获得交叉个体,均匀交叉
			PDDE::Decoding_Individual(ui);//解码新个体
			PDDE::Individual newxi = PDDE::Selection(xi, ui);//选择较好的个体进入下一代种群
			PDDE::initialpop[i] = newxi;
		}

		PDDE::Individual newbest = PDDE::Find_Best_Individual(PDDE::initialpop);//寻找新种群最优个体
		if (newbest.objective < best.objective)
			best = newbest;
		iteration_value.push_back(best.objective);

		stop = clock();//结束计时
		computingtime = (double)(stop - start) / CLOCKS_PER_SEC;//计算时间
	}

	//输出结果
	std::cout << "计算时间CPU seconds=" << computingtime << endl;
	std::cout << "实际迭代次数为：" << generation << endl;
	PDDE::Output_Individual_Information(best);

	// 保存结果
	result.computingtime = computingtime;
	result.generation = generation;
	result.best = best;
	result.iteration_value = iteration_value;
}

void PDDE::Call_PDDE_Variable_Heuristic_Neighborhood_determine_demand(int jobscase, int stagescase, int machinescase, int avgdemand, Result& result)
{
	//读取数据
	PDDE::jobs = jobscase + PDDE::Demand_Upper;
	PDDE::stages = stagescase;
	PDDE::machines = machinescase;
	PDDE::Read_Data();

	//将不确定性因素变成确定性
	for (int i = 0; i < PDDE::scenarios; i++)
		PDDE::Demand[i][PDDE::product_types - 1] = avgdemand;

	PDDE::Initial_Population_Variable_Heuristic(PDDE::initialpop, jobscase, PDDE::Demand_Lower, PDDE::Demand_Upper, PDDE::NP);//初始化种群
	PDDE::Individual best = PDDE::Find_Best_Individual(PDDE::initialpop);//记录最优个体

	clock_t start, stop;//计时器
	double computingtime = 0;//记录时间
	start = clock();//开始计时

	int generation = 0;//记录实际迭代次数
	while (computingtime < PDDE::TP * PDDE::jobs * PDDE::stages) //迭代终止条件
	{
		generation++;

		srand((unsigned)time(NULL));
		for (int i = 0; i < PDDE::initialpop.size(); i++)
		{
			PDDE::Individual xi = PDDE::initialpop[i];//当前个体
			PDDE::Individual vi = PDDE::Mutation_Current_1_Neighborhood(generation, xi, PDDE::initialpop, PDDE::F, best);//获得变异个体，PDDE/current/1+邻域搜索策略
			PDDE::Individual ui = PDDE::CrossOver_One_Point(xi, vi, PDDE::CR);//获得交叉个体,单点交叉
			//PDDE::Individual ui = PDDE::CrossOver_Uniform(xi, vi, PDDE::CR);//获得交叉个体,均匀交叉
			PDDE::Decoding_Individual(ui);//解码新个体
			PDDE::Individual newxi = PDDE::Selection(xi, ui);//选择较好的个体进入下一代种群
			PDDE::initialpop[i] = newxi;
		}

		PDDE::Individual newbest = PDDE::Find_Best_Individual(PDDE::initialpop);//寻找新种群最优个体
		if (newbest.objective < best.objective)
			best = newbest;
		
		stop = clock();//结束计时
		computingtime = (double)(stop - start) / CLOCKS_PER_SEC;//计算时间

	}

	//输出结果
	PDDE::Output_Individual_Information(best);

	// 保存结果
	result.best = best;
}

void PDDE::Call_PDDE_Variable_Heuristic_Neighborhood_determine_processtime(int jobscase, int stagescase, int machinescase, int avgprocesstime, Result& result)
{
	//读取数据
	PDDE::jobs = jobscase + PDDE::Demand_Upper;
	PDDE::stages = stagescase;
	PDDE::machines = machinescase;
	PDDE::Read_Data();

	//将不确定性因素变成确定性
	for (int n = 0; n < PDDE::scenarios; n++)
	{
		int jobindex = accumulate(PDDE::product_types_to_jobs.begin(), PDDE::product_types_to_jobs.end() - 1, 0);
		for (int y = 0; y < PDDE::product_types_to_jobs[PDDE::product_types - 1]; y++)
		{
			PDDE::processing_time[n][jobindex][PDDE::stages - 1] = avgprocesstime;
			jobindex++;
		}
	}

	PDDE::Initial_Population_Variable_Heuristic(PDDE::initialpop, jobscase, PDDE::Demand_Lower, PDDE::Demand_Upper, PDDE::NP);//初始化种群
	PDDE::Individual best = PDDE::Find_Best_Individual(PDDE::initialpop);//记录最优个体

	clock_t start, stop;//计时器
	double computingtime = 0;//记录时间
	start = clock();//开始计时

	int generation = 0;//记录实际迭代次数
	while (computingtime < PDDE::TP * PDDE::jobs * PDDE::stages) //迭代终止条件
	{
		generation++;

		srand((unsigned)time(NULL));
		for (int i = 0; i < PDDE::initialpop.size(); i++)
		{
			PDDE::Individual xi = PDDE::initialpop[i];//当前个体
			PDDE::Individual vi = PDDE::Mutation_Current_1_Neighborhood(generation, xi, PDDE::initialpop, PDDE::F, best);//获得变异个体，PDDE/current/1+邻域搜索策略
			PDDE::Individual ui = PDDE::CrossOver_One_Point(xi, vi, PDDE::CR);//获得交叉个体,单点交叉
			//PDDE::Individual ui = PDDE::CrossOver_Uniform(xi, vi, PDDE::CR);//获得交叉个体,均匀交叉
			PDDE::Decoding_Individual(ui);//解码新个体
			PDDE::Individual newxi = PDDE::Selection(xi, ui);//选择较好的个体进入下一代种群
			PDDE::initialpop[i] = newxi;
		}

		PDDE::Individual newbest = PDDE::Find_Best_Individual(PDDE::initialpop);//寻找新种群最优个体
		if (newbest.objective < best.objective)
			best = newbest;

		stop = clock();//结束计时
		computingtime = (double)(stop - start) / CLOCKS_PER_SEC;//计算时间

	}

	//输出结果
	PDDE::Output_Individual_Information(best);

	// 保存结果
	result.best = best;
}