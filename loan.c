#include <stdio.h>
#include <stdlib.h>

/* �ȶϢ
 * ÿ���»�����ܶ��ǹ̶�ֵ
 * P(n) - ��n����ʱ���ı���
 * S(n) - ��n���»���ǰʣ��ı���
 * r	- ������
 * ���У�
 * S(0)*r+P(0) = S(1)*r+P(1)
 * S(1) = S(0)-P(0)
 * =>S(0)*r+P(0) = S(0)*r-P(0)*r+P(1)
 * =>P(1) = P(0)*(1+r)
 * =>P(n) = P(0)*(1+r)^n
 * @param	sum			�ܶ�
 * @param	month		����
 * @param	rate_y		������
 */
void loan_debx(unsigned int sum, unsigned int month, double rate_y)
{
	unsigned int cnt;
	double		 rate_m = rate_y/12;	// ������
	double		 f_p, f_p_x, f_p_sum;
	double		 f_i_x, f_i_sum;
	double		 f_a_x, f_a_sum;
	double		 f_b_x;
	double		 f_tmp;
	FILE*		 fp;
	char		 buff[128];
	
	fp = fopen("debx.txt", "wb+");
	if(NULL == fp)
	{
		printf("open file <debx.txt> fail\r\n");
		return;
	}
	
	sprintf(buff, "�ȶϢ���[%d Ԫ] [%d ��] [����%.02f%%]\r\n", sum, month, rate_y*100);
	fwrite(buff, 1, strlen(buff), fp);
	printf("%s", buff);
	
	//				%03d : %5.02f ->[%5.02f] [%5.02f]\r\n
	sprintf(buff, "����   �¹�      ����      ��Ϣ\r\n");
	fwrite(buff, 1, strlen(buff), fp);
	printf("%s", buff);
	
	// ��������µĻ����Ե�һ���»�����Ȩ�ĺ�
	f_tmp = 0;
	for(cnt=0; cnt<month; cnt++)
	{
		f_tmp += pow(1+rate_m, cnt);
	}
	// �����һ�������(f_p)
	f_p = sum/f_tmp;
	
	f_b_x = sum;
	f_p_sum = 0;
	f_i_sum = 0;
	f_a_sum = 0;
	for(cnt=0; cnt<month; cnt++)
	{
		// ��ǰ�·ݻ����(f_p_x)
		f_p_x = f_p*pow(1+rate_m, cnt);
		// ��ǰ�·ݻ�����Ϣ(f_i_x)
		f_i_x = f_b_x*rate_m;
		// ��ǰ�·ݻ����ܶ�(f_a_x)
		f_a_x = f_p_x+f_i_x;
		
		sprintf(buff, "%03d : %5.02f ->[%5.02f] [%5.02f]\r\n", cnt+1, f_a_x, f_p_x, f_i_x);
		fwrite(buff, 1, strlen(buff), fp);
		printf("%s", buff);
		
		// �ѻ������ܶ�
		f_p_sum += f_p_x;
		// �ѻ���Ϣ�ܶ�
		f_i_sum += f_i_x;
		// �ѻ��ܶ�
		f_a_sum += f_a_x;
		// ʣ��δ������
		f_b_x -= f_p_x;
	}
	
	sprintf(buff, "ȫ������: %.02f, ����: %.02f, ��Ϣ: %.02f, ʣ�౾��%.02f\r\n", f_a_sum, f_p_sum, f_i_sum, f_b_x);
	fwrite(buff, 1, strlen(buff), fp);
	printf("%s", buff);
	
	fclose(fp);
}

/* �ȶ��
 * @param	sum			�ܶ�
 * @param	month		����
 * @param	rate_y		������
 */
void loan_debj(unsigned int sum, unsigned int month, double rate_y)
{
	unsigned int cnt;
	double		 rate_m = rate_y/12;
	double		 f_p, f_p_x, f_p_sum;
	double		 f_i_x, f_i_sum;
	double		 f_a_x, f_a_sum;
	double		 f_b_x;
	double		 f_tmp;
	FILE*		 fp;
	char		 buff[128];
	
	fp = fopen("debj.txt", "wb+");
	if(NULL == fp)
	{
		printf("open file <debj.txt> fail\r\n");
		return;
	}
	
	sprintf(buff, "�ȶ����[%d Ԫ] [%d ��] [����%.02f%%]\r\n", sum, month, rate_y*100);
	fwrite(buff, 1, strlen(buff), fp);
	printf("%s", buff);
	
	sprintf(buff, "����   �¹�      ����      ��Ϣ\r\n");
	fwrite(buff, 1, strlen(buff), fp);
	printf("%s", buff);
	
	f_p = (double)sum/(month);
	f_b_x = sum;
	f_p_sum = 0;
	f_i_sum = 0;
	f_a_sum = 0;
	for(cnt=0; cnt<month; cnt++)
	{
		f_p_x = f_p;
		f_i_x = f_b_x*rate_m;
		f_a_x = f_p_x+f_i_x;
		
		sprintf(buff, "%03d : %5.02f ->[%5.02f] [%5.02f]\r\n", cnt+1, f_a_x, f_p_x, f_i_x);
		fwrite(buff, 1, strlen(buff), fp);
		printf("%s", buff);
	
		f_p_sum += f_p_x;
		f_i_sum += f_i_x;
		f_a_sum += f_a_x;
		
		f_b_x -= f_p_x;
	}
	
	sprintf(buff, "ȫ������: %.02f, ����: %.02f, ��Ϣ: %.02f, ʣ�౾��%.02f\r\n", f_a_sum, f_p_sum, f_i_sum, f_b_x);
	fwrite(buff, 1, strlen(buff), fp);
	printf("%s", buff);
	
	fclose(fp);
}

int main(void)
{
	FILE* fp;
	unsigned int sum;
	unsigned int month;
	double        rate;
	char		 buff[128];
	char		 tmp[128];
	int			 tmp_index=0, flag=0;
	int 		 len, cnt;
	
	fp = fopen("cfg.txt", "r");
	if(NULL == fp)
	{
		printf("open <cfg.txt> fail\r\n");

		fp = fopen("..\\cfg.txt", "r");
		if(NULL == fp)
		{
			printf("open <..\\cfg.txt> fail\r\n");
			return 0; 
		}
	}
	
	len = fread(buff, 1, 127, fp);
	buff[len] = 0;
	
	if(len > 0)
	{
		for(cnt=0; cnt<len+1; cnt++)
		{
			if('\r'==buff[cnt] || '\n'==buff[cnt] || '\0' == buff[cnt])
			{
				// "\r\n"���������
				if(0==tmp_index)
					continue;

				tmp[tmp_index] = 0;
				switch(flag)
				{
					// �ܶ�
					case 0:
						sum = atoi(tmp);
						flag = 1;
						break;
					// ����	
					case 1:
						month = atoi(tmp);
						flag = 2;
						break;
					// ������	
					case 2:
						rate = atof(tmp);
						flag = 3;
						break;
					// ���������	
					case 3:
						flag = 3;
						break;
					default:
						flag = 0;
						break;
				}
				tmp_index = 0;
				if('\0' == buff[cnt])
				{
					break;
				}
			}
			else
			{
				tmp[tmp_index++] = buff[cnt];
			}
		}
		if(3 == flag)
		{
			printf("sum: %d, month: %d, rate: %f\r\n", sum, month, rate);
			if(0==month || 0==sum || 0==rate)
				return 0;
		}
		else
		{
			printf("Parse cfg.txt error(%u)\r\n", flag);
			return 0;
		}
	}
	fclose(fp);
	
	loan_debx(sum, month, rate);
	loan_debj(sum, month, rate);
}

