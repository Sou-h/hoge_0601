#include <stdio.h>
#include <stdlib.h>
/* 記号定数 */
#define NOPS 10		/*粒子の個数*/
#define LIMITL 256	/*配列最大領域*/
#define ILIMIT 5	/*繰り返しの回数*/
#define SEED 32767	/*乱数の初期値*/
#define W 0.3		/*慣性定数*/
#define C1 1.0		/*ローカルな質量*/
#define C2 1.0		/*グローバルな質量*/

/* 平面座標を表現する構造体 */
struct point{
	double x;
	double y;
};

/* 粒子を表現する構造体 */
struct particle{
	struct point pos;		/*位置*/
	double value;				/*評価値*/
	struct point v;			/*速度*/
	struct point bestpos;	/*最適位置*/
	double bestval;			/*最適評価値*/
};
/* 関数のプロトタイプの宣言 */
void initps(struct particle ps[]);
/*粒子群の初期化*/
void printps(struct particle ps[]);
/*粒子群の表示*/
double frand();						/*実数乱数*/
double calcval(double x, double y);	/*評価値の計算*/
void optimize(struct particle ps[]);
/*粒子群の位置更新*/
void setgbest(struct particle ps[]);
/* 群れの最適位置を格納 */

/*大域変数*/
struct point gbestpos;	/*群中の最適位置*/
double gbestval;		/*群中の最適評価値*/

/**********************/
/* main() 関数 */
/**********************/
int main()
{

	FILE *out_file_x, *out_file_y, *out_file_val;

	fopen_s(&out_file_x, "pso_1_x.txt","w");
	fopen_s(&out_file_y, "pso_1_y.txt", "w");
	fopen_s(&out_file_val, "pso_1_val.txt", "w");
	struct particle ps[LIMITL];	/*粒子群*/
	int i;						/*繰り返し回数の制御*/
	/*乱数の初期化*/
	srand(SEED);
	/*粒子群の初期化*/
	initps(ps);
	
	printps(ps);
	/*最適化の本体*/
	for (i = 0; i<ILIMIT; ++i){
		optimize(ps);
		//テキストに保存
		for (int count = 0; count < NOPS; count++){
			fprintf(out_file_x, "%lf\t",  ps[count].pos.x);
			fprintf(out_file_y, "%lf\t", ps[count].pos.y);
			fprintf(out_file_val, "%lf\t", ps[count].value);
		}	

		fprintf(out_file_x, "\n");
		fprintf(out_file_y, "\n");
		fprintf(out_file_val, "\n");
		printf("%d 回目\n", i+1);
		printps(ps);
	}

	char hoge;
	fclose(out_file_x);
	fclose(out_file_y);
	fclose(out_file_val);
	scanf_s("%d",&hoge);
	return 0;
}

/**************************/
/* optimize() 関数 */
/* 粒子群の位置更新 */
/**************************/
void optimize(struct particle ps[])
{
	int i;
	double r1, r2;/*乱数の値を格納*/
	for (i = 0; i<NOPS; ++i){
		/*乱数の設定*/
		r1 = frand();
		r2 = frand();
		/*速度の更新*/
		ps[i].v.x = W*ps[i].v.x
			+ C1*r1*(ps[i].bestpos.x - ps[i].pos.x)
			+ C2*r2*(gbestpos.x - ps[i].pos.x);
		ps[i].v.y = W*ps[i].v.y
			+ C1*r1*(ps[i].bestpos.y - ps[i].pos.y)
			+ C2*r2*(gbestpos.y - ps[i].pos.y);
		/*位置の更新*/
		ps[i].pos.x += ps[i].v.x;
		ps[i].pos.y += ps[i].v.y;
		


		/*最適値の更新*/
		ps[i].value = calcval(ps[i].pos.x, ps[i].pos.y);
		
		if (ps[i].value<ps[i].bestval){
			ps[i].bestval = ps[i].value;
			ps[i].bestpos = ps[i].pos;
		}
	}
	/*群中最適値の更新*/
	setgbest(ps);
}

/**************************/
/* calcbal() 関数 */
/* 評価値の計算 */
/**************************/
double calcval(double x, double y)
{
	double  val = x*x + y*y + 1;
	printf("sum=%.6lf\n", val);

	return val;
}

double calcval2(double x, double y)
{	
	double sum = 0;
	for (int i = 0; i < NOPS; i++){
		sum = sum + x *x;
	}

	return sum;
}

/**************************/
/* setgbest() 関数 */
/* 群れの最適位置を格納 */
/**************************/
void setgbest(struct particle ps[])
{
	int i;
	double besti;
	double x, y;
	besti = ps[0].value;
	x = ps[0].pos.x;
	y = ps[0].pos.y;
	for (i = 0; i<NOPS; ++i)
		/*現在の最良評価値を探す*/
	if (ps[i].value<besti){
		besti = ps[i].value;
		x = ps[i].pos.x;
		y = ps[i].pos.y;
	}
	
		/*評価値が過去よりもよかったら更新*/
	if (besti<gbestval){
		gbestval = besti;
		gbestpos.x = x;
		gbestpos.y = y;
	}
}

/**************************/
/* initps() 関数 */
/* 粒子群の初期化 */
/**************************/
void initps(struct particle ps[])
{
	int i;
	double x, y;
	for (i = 0; i<NOPS; ++i){
		/*位置*/
		x = ps[i].pos.x = frand() * 2 - 1.0;
		y = ps[i].pos.y = frand() * 2 - 1.0;
		/*評価値*/
		ps[i].value = calcval(x, y);
		/*速度*/
		ps[i].v.x = frand() * 2 - 1.0;
		ps[i].v.y = frand() * 2 - 1.0;
		/*最適位置*/
		ps[i].bestpos.x = ps[i].pos.x;
		ps[i].bestpos.y = ps[i].pos.y;
		/*最適評価値*/
		ps[i].bestval = ps[i].value;
	}

	/*群れの最適位置を格納*/
	gbestval	= ps[0].value;
	gbestpos.x	= ps[0].pos.x;
	gbestpos.y	= ps[0].pos.y;
	setgbest(ps);
}

/**************************/
/* printps() 関数 */
/* 粒子群の表示 */
/**************************/
void printps(struct particle ps[])
{
	int i;
	for (i = 0; i<NOPS; ++i){
		printf("粒子%d\t", i);
		printf("x=%.4lf\ty=%.4lf\t", ps[i].pos.x, ps[i].pos.y);
		printf("評価値%lf ", ps[i].value);
//		printf("%lf %lf ", ps[i].v.x, ps[i].v.y);
//		printf("%lf %lf ",ps[i].bestpos.x, ps[i].bestpos.y);
		printf("最適評価値%lf\n", ps[i].bestval);
	}
	printf("結果 x=%lf y=%lf val=%lf\n", gbestpos.x, gbestpos.y, gbestval);
}
/*************************/
/* frand() 関数 */
/* 実数乱数 */
/*************************/
double frand(void)
{
	double result;
	while ((result = (double)rand() / RAND_MAX) >= 1);
	return result;
}