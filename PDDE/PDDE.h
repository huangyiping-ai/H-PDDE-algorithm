//PDDE.h��PDDE�㷨����

#pragma once
#include "pch.h"
using namespace std;

class PDDE
{
public:
	/****************************************��ѧģ�͵Ĳ���****************************************/
	int scenarios;//������
	int Demand_Lower;//������ȷ��ʱ����ȷ�����������½磬ֻ������ȷ��ʱ����Ҫ��ֵ������Ϊ0
	int Demand_Upper;//������ȷ��ʱ����ȷ�����������Ͻ磬ֻ������ȷ��ʱ����Ҫ��ֵ������Ϊ0
	int jobs;//��ҵ��
	int stages;//�׶���
	int machines;//ÿ���׶εĲ��л�����
	int product_types;//��Ʒ������
	vector<int> product_types_to_jobs;//���ڲ�Ʒ����c����ҵ��

	vector<double> probability;//����s���ֵĸ���
	vector<int> processing_time_uncertain;//����s�����һ�ֲ�Ʒ����c�����һ���׶�k�ļӹ�ʱ��
	vector<vector<vector<int>>> processing_time;//����s����ҵj�ڽ׶�k�ļӹ�ʱ��
	vector<int> Demand_uncertain;// ����s�����һ�ֲ�Ʒ���͵�������
	vector<vector<int>> Demand;//����s�²�Ʒ����c��������
	vector<int> due_date;//��ҵj�ĵ����գ������������ȷֲ�
	vector<double> unit_processing_cost;//��Ʒ����c�ĵ�λ�����ɱ�
	vector<double> unit_holding_cost;//��Ʒ����c�ĵ�λ���ɱ�
	vector<double> unit_external_production_cost;////��Ʒ����c�ĵ�λ�ⲿ�����ɱ�
	vector<double> unit_delayed_completion_cost;//��ҵj�ĵ�λ�����깤�ɱ�

	/****************************************PDDE�㷨�Ĳ���****************************************/
	int NP;//��Ⱥ��ģ
	int Gmax;//����������		
	double F;//����ϵ��
	double CR;//�������
	int TP;//������ֹ������ʱ�����

	/****************************************���ڱ������ı���****************************************/
	struct Individual //������Ϣ
	{
		vector<int> sequence;//�����������
		double objective;//Ŀ�꺯��ֵ����Ϊ����С�����⣬����ԽСԽ��
		vector<vector<vector<vector<int>>>> starttime;//ÿ����ҵ��ÿ���׶ε�ÿ̨�����ϵĿ�ʼʱ��
		vector<vector<vector<vector<int>>>> finishtime;//ÿ����ҵ��ÿ���׶ε�ÿ̨�����ϵ����ʱ��
		vector<vector<int>> lasttime;//ÿ����ҵ�뿪���һ�������ʱ�䣬��Ӧ��ҵ���[0,1,2,3,4,5,����]
		vector<int> makespan;//������
		double expect_makespan;//�����ڵ�����
		vector<double> total_cost;//�ܳɱ�
		double expect_total_cost;//�ܳɱ�������
	};
	vector<Individual> initialpop;//������Ⱥ���Լ�������Ⱥ������ÿ��ѡ����������ĸ���
	struct Result //ÿ�ε����㷨�����ڴ�Ž��
	{
		double computingtime;//����ʱ��
		int generation;//ʵ�ʵ�������
		Individual best;//���Ÿ���
		vector<double> iteration_value;//ÿ�ε�����Ŀ�꺯��ֵ
	};

public:
	/****************************************���캯��****************************************/
	PDDE();
	~PDDE();

	/****************************************����Ԥ������غ���****************************************/
	void Read_Data();//��ȡ����

	/****************************************��ʼ����غ���****************************************/
	void Initial_Population(vector<Individual>& pop, int jobnum, int NPnum);//��ʼ����Ⱥ��������볤����ͬ
	void Initial_Population_Variable(vector<Individual>& pop, int jobnum, int Lower, int Upper, int NPnum);//��ʼ����Ⱥ��������볤�ȿɱ�
	void Initial_Population_Variable_Heuristic(vector<Individual>& pop, int jobnum, int Lower, int Upper, int NPnum);//��ʼ����Ⱥ��������볤�ȿɱ�+����ʽ�㷨��ʼ��Ⱥ

	/****************************************������غ���****************************************/
	void Decoding_Individual(Individual& person);//�������
	void Makespan(Individual& person, int ordernumber, int parallel, vector<vector<int>> usetime);//�������깤�ȼӹ�ԭ�򣬵õ�ÿ����ҵ��ÿ���׶ε�ÿ̨�����ϵĿ�ʼ����ɣ�ʱ�䣬�Լ�������
	void BubbleSort(vector<int>& x, vector<int>& y);//ð������
	void Total_Cost(Individual& person, vector<int> demand, vector<int> lasttime);//�����ܳɱ�

	/****************************************���������غ���****************************************/
	// ������Բ���PDDE / current / 1
	Individual Mutation_Current_1(Individual person_current, vector<Individual> pop, double factor);
	//������Բ���PDDE/current/1+������������
	Individual Mutation_Current_1_Neighborhood(int generation, Individual person_current, vector<Individual> pop, double factor, Individual best);
	vector<int> MinusOperation(vector<int> x, vector<int> y);//�Զ����������
	vector<int> AddOperation(vector<int> x, vector<int> y);//�Զ���ӷ�����
	vector<int> Uniform_Length(vector<int> x, vector<int> y);//ͳһ������볤�ȣ����볤��һ�£��������Զ���������ӷ�����
	vector<int> Swap(vector<int> x);//�ֲ�������������������
	vector<int> Insert(vector<int> x);//�ֲ������������������

	/****************************************���������غ���****************************************/
	Individual CrossOver_One_Point(Individual person1, Individual person2, double factor);//������Բ��õ��㽻��
	vector<int> Overlapping(vector<int> x, vector<int> y);//�Զ��彻�����
	int Find_Repeat_Index(vector<int> x);//Ѱ�ҵ�һ���ظ�Ԫ�ص���������������1234321������4
	Individual CrossOver_Uniform(Individual person1, Individual person2, double factor);//������Բ��þ��Ƚ���

	/****************************************ѡ�������غ���****************************************/
	Individual Selection(Individual person1, Individual person2);//ѡ����ԣ�ѡ�������һ���ĸ���

	/****************************************��������غ���****************************************/
	Individual Find_Best_Individual(vector<Individual> pop);//Ѱ����Ⱥ�е����Ÿ���
	void Output_Individual_Information(Individual person);//���������Ϣ
	double Standard(double avg, vector<PDDE::Result> message);//�����׼��

	/****************************************���ɹ����VND�������ʽ�㷨����غ���****************************************/
	void EDD_LPT(vector<int> x, vector<int>& y);//���ɹ�������ִ��EDD��Ȼ���ڵ�������ͬ������£�ִ��LPT
	Individual Find_Neighborhood(int structure, vector<int> x);//Ѱ��ĳ������ṹ���������Ÿ���

	/****************************************�������㷨�ĺ���������滮ģ��****************************************/
	//���ɹ����VND��ϵ�����ʽ�㷨
	void Call_Heuristic(int jobscase, int stagescase, int machinescase, Result& result);
	//����PDDE�㷨
	void Call_PDDE(int jobscase, int stagescase, int machinescase, Result& result);
	//����PDDE�㷨+������볤�ȿɱ����
	void Call_PDDE_Variable(int jobscase, int stagescase, int machinescase, Result& result);
	//����PDDE�㷨+������볤�ȿɱ����+������������
	void Call_PDDE_Variable_Neighborhood(int jobscase, int stagescase, int machinescase, Result& result);
	//����PDDE�㷨+������볤�ȿɱ����+����ʽ�㷨��ʼ��Ⱥ
	void Call_PDDE_Variable_Heuristic(int jobscase, int stagescase, int machinescase, Result& result);
	//����PDDE�㷨+������볤�ȿɱ����+����ʽ�㷨��ʼ��Ⱥ+�����������ԣ�����滮ģ��
	void Call_PDDE_Variable_Heuristic_Neighborhood(int jobscase, int stagescase, int machinescase, Result& result);

	/****************************************�������㷨�ĺ�����ȷ����ģ��****************************************/
	//����PDDE�㷨+������볤�ȿɱ����+����ʽ�㷨��ʼ��Ⱥ+�����������ԣ�ȷ����ģ�ͣ�������̶�
	void Call_PDDE_Variable_Heuristic_Neighborhood_determine_demand(int jobscase, int stagescase, int machinescase, int avgdemand, Result& result);
	//����PDDE�㷨+������볤�ȿɱ����+����ʽ�㷨��ʼ��Ⱥ+�����������ԣ�ȷ����ģ�ͣ��Ѽӹ�ʱ��̶�
	void Call_PDDE_Variable_Heuristic_Neighborhood_determine_processtime(int jobscase, int stagescase, int machinescase, int avgprocesstime, Result& result);
};