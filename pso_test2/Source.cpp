#include <stdio.h>
#include <stdlib.h>
/* �L���萔 */
#define NOPS 10		/*���q�̌�*/
#define LIMITL 256	/*�z��ő�̈�*/
#define ILIMIT 5	/*�J��Ԃ��̉�*/
#define SEED 32767	/*�����̏����l*/
#define W 0.3		/*�����萔*/
#define C1 1.0		/*���[�J���Ȏ���*/
#define C2 1.0		/*�O���[�o���Ȏ���*/

/* ���ʍ��W��\������\���� */
struct point{
	double x;
	double y;
};

/* ���q��\������\���� */
struct particle{
	struct point pos;		/*�ʒu*/
	double value;				/*�]���l*/
	struct point v;			/*���x*/
	struct point bestpos;	/*�œK�ʒu*/
	double bestval;			/*�œK�]���l*/
};
/* �֐��̃v���g�^�C�v�̐錾 */
void initps(struct particle ps[]);
/*���q�Q�̏�����*/
void printps(struct particle ps[]);
/*���q�Q�̕\��*/
double frand();						/*��������*/
double calcval(double x, double y);	/*�]���l�̌v�Z*/
void optimize(struct particle ps[]);
/*���q�Q�̈ʒu�X�V*/
void setgbest(struct particle ps[]);
/* �Q��̍œK�ʒu���i�[ */

/*���ϐ�*/
struct point gbestpos;	/*�Q���̍œK�ʒu*/
double gbestval;		/*�Q���̍œK�]���l*/

/**********************/
/* main() �֐� */
/**********************/
int main()
{

	FILE *out_file_x, *out_file_y, *out_file_val;

	fopen_s(&out_file_x, "pso_1_x.txt","w");
	fopen_s(&out_file_y, "pso_1_y.txt", "w");
	fopen_s(&out_file_val, "pso_1_val.txt", "w");
	struct particle ps[LIMITL];	/*���q�Q*/
	int i;						/*�J��Ԃ��񐔂̐���*/
	/*�����̏�����*/
	srand(SEED);
	/*���q�Q�̏�����*/
	initps(ps);
	
	printps(ps);
	/*�œK���̖{��*/
	for (i = 0; i<ILIMIT; ++i){
		optimize(ps);
		//�e�L�X�g�ɕۑ�
		for (int count = 0; count < NOPS; count++){
			fprintf(out_file_x, "%lf\t",  ps[count].pos.x);
			fprintf(out_file_y, "%lf\t", ps[count].pos.y);
			fprintf(out_file_val, "%lf\t", ps[count].value);
		}	

		fprintf(out_file_x, "\n");
		fprintf(out_file_y, "\n");
		fprintf(out_file_val, "\n");
		printf("%d ���\n", i+1);
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
/* optimize() �֐� */
/* ���q�Q�̈ʒu�X�V */
/**************************/
void optimize(struct particle ps[])
{
	int i;
	double r1, r2;/*�����̒l���i�[*/
	for (i = 0; i<NOPS; ++i){
		/*�����̐ݒ�*/
		r1 = frand();
		r2 = frand();
		/*���x�̍X�V*/
		ps[i].v.x = W*ps[i].v.x
			+ C1*r1*(ps[i].bestpos.x - ps[i].pos.x)
			+ C2*r2*(gbestpos.x - ps[i].pos.x);
		ps[i].v.y = W*ps[i].v.y
			+ C1*r1*(ps[i].bestpos.y - ps[i].pos.y)
			+ C2*r2*(gbestpos.y - ps[i].pos.y);
		/*�ʒu�̍X�V*/
		ps[i].pos.x += ps[i].v.x;
		ps[i].pos.y += ps[i].v.y;
		


		/*�œK�l�̍X�V*/
		ps[i].value = calcval(ps[i].pos.x, ps[i].pos.y);
		
		if (ps[i].value<ps[i].bestval){
			ps[i].bestval = ps[i].value;
			ps[i].bestpos = ps[i].pos;
		}
	}
	/*�Q���œK�l�̍X�V*/
	setgbest(ps);
}

/**************************/
/* calcbal() �֐� */
/* �]���l�̌v�Z */
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
/* setgbest() �֐� */
/* �Q��̍œK�ʒu���i�[ */
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
		/*���݂̍ŗǕ]���l��T��*/
	if (ps[i].value<besti){
		besti = ps[i].value;
		x = ps[i].pos.x;
		y = ps[i].pos.y;
	}
	
		/*�]���l���ߋ������悩������X�V*/
	if (besti<gbestval){
		gbestval = besti;
		gbestpos.x = x;
		gbestpos.y = y;
	}
}

/**************************/
/* initps() �֐� */
/* ���q�Q�̏����� */
/**************************/
void initps(struct particle ps[])
{
	int i;
	double x, y;
	for (i = 0; i<NOPS; ++i){
		/*�ʒu*/
		x = ps[i].pos.x = frand() * 2 - 1.0;
		y = ps[i].pos.y = frand() * 2 - 1.0;
		/*�]���l*/
		ps[i].value = calcval(x, y);
		/*���x*/
		ps[i].v.x = frand() * 2 - 1.0;
		ps[i].v.y = frand() * 2 - 1.0;
		/*�œK�ʒu*/
		ps[i].bestpos.x = ps[i].pos.x;
		ps[i].bestpos.y = ps[i].pos.y;
		/*�œK�]���l*/
		ps[i].bestval = ps[i].value;
	}

	/*�Q��̍œK�ʒu���i�[*/
	gbestval	= ps[0].value;
	gbestpos.x	= ps[0].pos.x;
	gbestpos.y	= ps[0].pos.y;
	setgbest(ps);
}

/**************************/
/* printps() �֐� */
/* ���q�Q�̕\�� */
/**************************/
void printps(struct particle ps[])
{
	int i;
	for (i = 0; i<NOPS; ++i){
		printf("���q%d\t", i);
		printf("x=%.4lf\ty=%.4lf\t", ps[i].pos.x, ps[i].pos.y);
		printf("�]���l%lf ", ps[i].value);
//		printf("%lf %lf ", ps[i].v.x, ps[i].v.y);
//		printf("%lf %lf ",ps[i].bestpos.x, ps[i].bestpos.y);
		printf("�œK�]���l%lf\n", ps[i].bestval);
	}
	printf("���� x=%lf y=%lf val=%lf\n", gbestpos.x, gbestpos.y, gbestval);
}
/*************************/
/* frand() �֐� */
/* �������� */
/*************************/
double frand(void)
{
	double result;
	while ((result = (double)rand() / RAND_MAX) >= 1);
	return result;
}