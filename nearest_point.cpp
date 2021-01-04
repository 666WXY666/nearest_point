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

bool compare_x(const point& A, const point& B)//比较x坐标，A的x小返回true，A和B相等或B的x小返回false
{
	return A.x < B.x;
}

bool compare_y(const point& A, const point& B)//比较y坐标，A的y小返回true，A和B的y相等或B的y小返回false
{
	return A.y < B.y;
}

double distance(const point& A, const point& B)//返回A和B之间的距离
{
	return sqrt(pow(A.x - B.x, 2) + pow(A.y - B.y, 2));
}

double merge(vector<point>& points_y, double min_d, int mid, pair<point, point>& min_point)//合并函数，将左右区域距离的最小值与中间区域的6个点比较
{																						//points为点的集合；dis为左右的最近点距离；mid为x坐标排序后，点集合中中间点的索引值；min_point为求得的最近点对
	vector<point> left, right;
	for (int i = 0; i < points_y.size(); i++)//搜集左右两边符合条件的点
	{
		if (points_y[i].x <= points_y[mid].x && points_y[i].x > points_y[mid].x - min_d)
			left.push_back(points_y[i]);
		else if (points_y[i].x > points_y[mid].x && points_y[i].x < points_y[mid].x + min_d)
			right.push_back(points_y[i]);
	}
	int j = 0;//右侧点的下界
	for (int i = 0; i < left.size(); i++)//遍历左边的点集合，与右边符合条件的计算距离
	{
		for (; j < right.size(); j++)
		{
			if (right[j].y >= left[i].y - min_d)
			{
				break;
			}
		}
		for (int l = 0; l < 6 && l + j < right.size(); l++)//遍历右边的6个点
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

double closest_point(vector<point>& points, vector<point>& points_y,pair<point,point> &min_point)//递归求解points中的最近点对，返回最近点对距离，min_point存储这个点对
{
	if (points.size() == 2)//两个点
	{
		min_point.first = points[0];
		min_point.second = points[1];
		return distance(points[0], points[1]);
	}	
	if (points.size() == 3)//三个点
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
	int mid = (points.size() >> 1) - 1;//size为偶数，mid为中点左边；size为奇数，mid为中点-1
	double d_left, d_right, min_d;
	vector<point> left(mid + 1), right(points.size() - mid - 1), left_y, right_y;//定义两个vector：left，right
	copy(points.begin(), points.begin() + mid + 1, left.begin());//复制左边区域点集合到left
	copy(points.begin() + mid + 1, points.end(), right.begin());//复制右边区域点集合到right
	for (int i = 0; i < points_y.size(); i++)//将按y排好序的数组以point[mid]分为两部分，分完后还是按y排好序的
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

double closest_point2(vector<point> p, pair<point, point>& min_point)//暴力穷举法
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
	cout << "请输入点的个数:";
	cin >> count;
	for (int i = 0; i < count; i++)
	{
		printf("第%d个点:", i+1);
		point p(u(e), u(e));
		printf("(%lf,%lf)\n", p.x, p.y);
		points.push_back(p);
		points_y.push_back(p);
	}
	sort(points.begin(), points.end(), compare_x);//把所有的点按x从小到大排序
	sort(points_y.begin(), points_y.end(), compare_y);//把左边的点按y从小到大排序
	min_distance = closest_point(points, points_y, min_point);
	cout << "分治法：" << endl;
	printf("最近点对为:(%lf,%lf),(%lf,%lf)\n最近点对距离为:%lf\n", min_point.first.x, min_point.first.y, min_point.second.x, min_point.second.y, min_distance);
	cout << "暴力穷举法：" << endl;
	min_distance = closest_point2(points, min_point);
	printf("最近点对为:(%lf,%lf),(%lf,%lf)\n最近点对距离为:%lf\n", min_point.first.x, min_point.first.y, min_point.second.x, min_point.second.y, min_distance);
	system("pause");
	return 0;
}
