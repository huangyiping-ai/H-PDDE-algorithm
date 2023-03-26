//PDDE.h：PDDE算法的类

#pragma once
#include "pch.h"
using namespace std;

class PDDE
{
public:
	/****************************************数学模型的参数****************************************/
	int scenarios;//场景数
	int Demand_Lower;//当需求不确定时，不确定需求量的下界，只有需求不确定时才需要赋值，否则为0
	int Demand_Upper;//当需求不确定时，不确定需求量的上界，只有需求不确定时才需要赋值，否则为0
	int jobs;//作业数
	int stages;//阶段数
	int machines;//每个阶段的并行机个数
	int product_types;//产品种类数
	vector<int> product_types_to_jobs;//属于产品种类c的作业数

	vector<double> probability;//场景s出现的概率
	vector<int> processing_time_uncertain;//场景s下最后一种产品类型c在最后一个阶段k的加工时间
	vector<vector<vector<int>>> processing_time;//场景s下作业j在阶段k的加工时间
	vector<int> Demand_uncertain;// 场景s下最后一种产品类型的需求量
	vector<vector<int>> Demand;//场景s下产品类型c的需求量
	vector<int> due_date;//作业j的到期日，服从整数均匀分布
	vector<double> unit_processing_cost;//产品类型c的单位生产成本
	vector<double> unit_holding_cost;//产品类型c的单位库存成本
	vector<double> unit_external_production_cost;////产品类型c的单位外部生产成本
	vector<double> unit_delayed_completion_cost;//作业j的单位延期完工成本

	/****************************************PDDE算法的参数****************************************/
	int NP;//种群规模
	int Gmax;//最大迭代次数		
	double F;//变异系数
	double CR;//交叉参数
	int TP;//迭代终止条件的时间参数

	/****************************************用于保存结果的变量****************************************/
	struct Individual //个体信息
	{
		vector<int> sequence;//个体编码排序
		double objective;//目标函数值，因为是最小化问题，所以越小越好
		vector<vector<vector<vector<int>>>> starttime;//每项作业在每个阶段的每台机器上的开始时间
		vector<vector<vector<vector<int>>>> finishtime;//每项作业在每个阶段的每台机器上的完成时间
		vector<vector<int>> lasttime;//每项作业离开最后一道工序的时间，对应作业序号[0,1,2,3,4,5,……]
		vector<int> makespan;//制造期
		double expect_makespan;//制造期的期望
		vector<double> total_cost;//总成本
		double expect_total_cost;//总成本的期望
	};
	vector<Individual> initialpop;//初代种群，以及迭代种群，放置每次选择后存活下来的个体
	struct Result //每次调用算法后，用于存放结果
	{
		double computingtime;//运行时间
		int generation;//实际迭代次数
		Individual best;//最优个体
		vector<double> iteration_value;//每次迭代的目标函数值
	};

public:
	/****************************************构造函数****************************************/
	PDDE();
	~PDDE();

	/****************************************数据预处理相关函数****************************************/
	void Read_Data();//读取数据

	/****************************************初始化相关函数****************************************/
	void Initial_Population(vector<Individual>& pop, int jobnum, int NPnum);//初始化种群，个体编码长度相同
	void Initial_Population_Variable(vector<Individual>& pop, int jobnum, int Lower, int Upper, int NPnum);//初始化种群，个体编码长度可变
	void Initial_Population_Variable_Heuristic(vector<Individual>& pop, int jobnum, int Lower, int Upper, int NPnum);//初始化种群，个体编码长度可变+启发式算法初始种群

	/****************************************解码相关函数****************************************/
	void Decoding_Individual(Individual& person);//个体解码
	void Makespan(Individual& person, int ordernumber, int parallel, vector<vector<int>> usetime);//根据先完工先加工原则，得到每项作业在每个阶段的每台机器上的开始（完成）时间，以及制造期
	void BubbleSort(vector<int>& x, vector<int>& y);//冒泡排序
	void Total_Cost(Individual& person, vector<int> demand, vector<int> lasttime);//计算总成本

	/****************************************变异策略相关函数****************************************/
	// 变异策略采用PDDE / current / 1
	Individual Mutation_Current_1(Individual person_current, vector<Individual> pop, double factor);
	//变异策略采用PDDE/current/1+邻域搜索策略
	Individual Mutation_Current_1_Neighborhood(int generation, Individual person_current, vector<Individual> pop, double factor, Individual best);
	vector<int> MinusOperation(vector<int> x, vector<int> y);//自定义减法操作
	vector<int> AddOperation(vector<int> x, vector<int> y);//自定义加法操作
	vector<int> Uniform_Length(vector<int> x, vector<int> y);//统一个体编码长度，编码长度一致，才能做自定义减法、加法操作
	vector<int> Swap(vector<int> x);//局部邻域搜索，交换操作
	vector<int> Insert(vector<int> x);//局部邻域搜索，插入操作

	/****************************************交叉策略相关函数****************************************/
	Individual CrossOver_One_Point(Individual person1, Individual person2, double factor);//交叉策略采用单点交叉
	vector<int> Overlapping(vector<int> x, vector<int> y);//自定义交叉操作
	int Find_Repeat_Index(vector<int> x);//寻找第一个重复元素的索引，例如输入1234321，返回4
	Individual CrossOver_Uniform(Individual person1, Individual person2, double factor);//交叉策略采用均匀交叉

	/****************************************选择策略相关函数****************************************/
	Individual Selection(Individual person1, Individual person2);//选择策略，选择进入下一代的个体

	/****************************************输出结果相关函数****************************************/
	Individual Find_Best_Individual(vector<Individual> pop);//寻找种群中的最优个体
	void Output_Individual_Information(Individual person);//输出个体信息
	double Standard(double avg, vector<PDDE::Result> message);//计算标准差

	/****************************************分派规则和VND结合启发式算法的相关函数****************************************/
	void EDD_LPT(vector<int> x, vector<int>& y);//分派规则，首先执行EDD，然后在到货期相同的情况下，执行LPT
	Individual Find_Neighborhood(int structure, vector<int> x);//寻找某种邻域结构产生的最优个体

	/****************************************调用主算法的函数，随机规划模型****************************************/
	//分派规则和VND结合的启发式算法
	void Call_Heuristic(int jobscase, int stagescase, int machinescase, Result& result);
	//基础PDDE算法
	void Call_PDDE(int jobscase, int stagescase, int machinescase, Result& result);
	//基础PDDE算法+个体编码长度可变策略
	void Call_PDDE_Variable(int jobscase, int stagescase, int machinescase, Result& result);
	//基础PDDE算法+个体编码长度可变策略+邻域搜索策略
	void Call_PDDE_Variable_Neighborhood(int jobscase, int stagescase, int machinescase, Result& result);
	//基础PDDE算法+个体编码长度可变策略+启发式算法初始种群
	void Call_PDDE_Variable_Heuristic(int jobscase, int stagescase, int machinescase, Result& result);
	//基础PDDE算法+个体编码长度可变策略+启发式算法初始种群+邻域搜索策略，随机规划模型
	void Call_PDDE_Variable_Heuristic_Neighborhood(int jobscase, int stagescase, int machinescase, Result& result);

	/****************************************调用主算法的函数：确定性模型****************************************/
	//基础PDDE算法+个体编码长度可变策略+启发式算法初始种群+邻域搜索策略，确定性模型，把需求固定
	void Call_PDDE_Variable_Heuristic_Neighborhood_determine_demand(int jobscase, int stagescase, int machinescase, int avgdemand, Result& result);
	//基础PDDE算法+个体编码长度可变策略+启发式算法初始种群+邻域搜索策略，确定性模型，把加工时间固定
	void Call_PDDE_Variable_Heuristic_Neighborhood_determine_processtime(int jobscase, int stagescase, int machinescase, int avgprocesstime, Result& result);
};