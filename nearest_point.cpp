#include<iostream>
#include<vector>
#include<algorithm>
#include<ctime>
#include<random>
using namespace std;

class point 
{
public:
	double x;
	double y;
	point(double x, double y) :x(x), y(y) {}
	point() { return; }
};

bool compare_x(const point& A, const point& B)//�Ƚ�x���꣬A��xС����true��A��B��Ȼ�B��xС����false
{
	return A.x < B.x;
}

bool compare_y(const point& A, const point& B)//�Ƚ�y���꣬A��yС����true��A��B��y��Ȼ�B��yС����false
{
	return A.y < B.y;
}

double distance(const point& A, const point& B)//����A��B֮��ľ���
{
	return sqrt(pow(A.x - B.x, 2) + pow(A.y - B.y, 2));
}

double merge(vector<point>& points_y, double min_d, int mid, pair<point, point>& min_point)//�ϲ�����������������������Сֵ���м������6����Ƚ�
{																						//pointsΪ��ļ��ϣ�disΪ���ҵ��������룻midΪx��������󣬵㼯�����м�������ֵ��min_pointΪ��õ�������
	vector<point> left, right;
	for (int i = 0; i < points_y.size(); i++)//�Ѽ��������߷��������ĵ�
	{
		if (points_y[i].x <= points_y[mid].x && points_y[i].x > points_y[mid].x - min_d)
			left.push_back(points_y[i]);
		else if (points_y[i].x > points_y[mid].x && points_y[i].x < points_y[mid].x + min_d)
			right.push_back(points_y[i]);
	}
	int j = 0;//�Ҳ����½�
	for (int i = 0; i < left.size(); i++)//������ߵĵ㼯�ϣ����ұ߷��������ļ������
	{
		for (; j < right.size(); j++)
		{
			if (right[j].y >= left[i].y - min_d)
			{
				break;
			}
		}
		for (int l = 0; l < 6 && l + j < right.size(); l++)//�����ұߵ�6����
		{
			if (distance(left[i], right[j+l]) < min_d)
			{
				min_d = distance(left[i], right[j+l]);
				min_point.first = left[i];
				min_point.second = right[j+l];
			}
		}
	}
	return min_d;
}

double closest_point(vector<point>& points, vector<point>& points_y,pair<point,point> &min_point)//�ݹ����points�е������ԣ����������Ծ��룬min_point�洢������
{
	if (points.size() == 2)//������
	{
		min_point.first = points[0];
		min_point.second = points[1];
		return distance(points[0], points[1]);
	}	
	if (points.size() == 3)//������
	{
		double min_distance = min(distance(points[0], points[1]), min(distance(points[0], points[2]), distance(points[1], points[2])));
		if (distance(points[0], points[1])==min_distance)
		{
			min_point.first = points[0];
			min_point.second = points[1];
		}
		else if(distance(points[0], points[2]) == min_distance)
		{
			min_point.first = points[0];
			min_point.second = points[2];
		}
		else
		{
			min_point.first = points[1];
			min_point.second = points[2];
		}
		return min_distance;
	}
	pair<point, point> temp_min_point1, temp_min_point2;
	int mid = (points.size() >> 1) - 1;//sizeΪż����midΪ�е���ߣ�sizeΪ������midΪ�е�-1
	double d_left, d_right, min_d;
	vector<point> left(mid + 1), right(points.size() - mid - 1), left_y, right_y;//��������vector��left��right
	copy(points.begin(), points.begin() + mid + 1, left.begin());//�����������㼯�ϵ�left
	copy(points.begin() + mid + 1, points.end(), right.begin());//�����ұ�����㼯�ϵ�right
	for (int i = 0; i < points_y.size(); i++)//����y�ź����������point[mid]��Ϊ�����֣�������ǰ�y�ź����
	{
		if (points_y[i].x <= points[mid].x)
			left_y.push_back(points_y[i]);
		else
			right_y.push_back(points_y[i]);
	}
	d_left = closest_point(left, left_y, temp_min_point1);
	d_right = closest_point(right, right_y, temp_min_point2);
	min_d = min(d_left, d_right);
	if (d_left == min_d)
	{
		min_point = temp_min_point1;
	}
	else
	{
		min_point = temp_min_point2;
	}
	return merge(points_y, min_d, mid, min_point);
}

double closest_point2(vector<point> p, pair<point, point>& min_point)//������ٷ�
{
	double minDistance = 99999;
	for (int i = 0; i < p.size(); i++)
	{
		for (int j = 0; j < p.size(); j++)
		{
			double d = distance(p[i], p[j]);
			if (d - 0 < 1e-4)
			{
				continue;
			}
			if (d < minDistance)
			{
				minDistance = d;
				min_point = make_pair(p[i], p[j]);
			}
		}
	}
	return minDistance;
}
int main()
{
	int count;
	double x, y, min_distance;
	vector<point> points;
	vector<point> points_y;
	pair<point, point> min_point;
	default_random_engine e(time(NULL));
	uniform_real_distribution<double> u(-10, 10);
	cout << "�������ĸ���:";
	cin >> count;
	for (int i = 0; i < count; i++)
	{
		printf("��%d����:", i+1);
		point p(u(e), u(e));
		printf("(%lf,%lf)\n", p.x, p.y);
		points.push_back(p);
		points_y.push_back(p);
	}
	sort(points.begin(), points.end(), compare_x);//�����еĵ㰴x��С��������
	sort(points_y.begin(), points_y.end(), compare_y);//����ߵĵ㰴y��С��������
	min_distance = closest_point(points, points_y, min_point);
	cout << "���η���" << endl;
	printf("������Ϊ:(%lf,%lf),(%lf,%lf)\n�����Ծ���Ϊ:%lf\n", min_point.first.x, min_point.first.y, min_point.second.x, min_point.second.y, min_distance);
	cout << "������ٷ���" << endl;
	min_distance = closest_point2(points, min_point);
	printf("������Ϊ:(%lf,%lf),(%lf,%lf)\n�����Ծ���Ϊ:%lf\n", min_point.first.x, min_point.first.y, min_point.second.x, min_point.second.y, min_distance);
	system("pause");
	return 0;
}
