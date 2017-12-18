/// @param	sum			总额
/// @param	year		年限
/// @param	rate_y		年利率
/// S(0)*r+P(0) = S(1)*r+P(1)
/// S(1) = S(0)-P(0)   
/// =>S(0)*r+P(0) = S(0)*r-P(0)*r+P(1)
/// =>P(1) = P(0)*(1+r)
/// =>P(n) = P(0)*(1+r)^n
/// P(n) - 第n个月时还的本金
/// S(n) - 第n个月还款前剩余的本金

#include <stdio.h>
#include <stdlib.h>

/// 等额本息
void loan_debx(unsigned int sum, unsigned int year, float rate_y)
{
	unsigned int cnt;
	float		 rate_m = rate_y/12;
	float		 f_p, f_p_x, f_p_sum;
	float		 f_i_x, f_i_sum;
	float		 f_a_x, f_a_sum;
	float		 f_b_x;
	float		 f_tmp;
	FILE*		 fp;
	char		 buff[128];
	
	fp = fopen("debx.txt", "wb+");
	if(NULL == fp)
	{
		printf("open file <debx.txt> fail\r\n");
		return;
	}
	
	sprintf(buff, "等额本息借款[%d 元] [%d 年] [利率%.02f%%]\r\n", sum, year, rate_y*100);
	fwrite(buff, 1, strlen(buff), fp);
	printf("%s", buff);
	
	//				%03d : %5.02f ->[%5.02f] [%5.02f]\r\n
	sprintf(buff, "分期   月供      本金      利息\r\n", sum, year, rate_y*100);
	fwrite(buff, 1, strlen(buff), fp);
	printf("%s", buff);
	
	f_tmp = 0;
	for(cnt=0; cnt<12*year; cnt++)
	{
		f_tmp += pow(1+rate_m, cnt);
	}
	f_p = sum/f_tmp;
	
	f_b_x = sum;
	f_p_sum = 0;
	f_i_sum = 0;
	f_a_sum = 0;
	for(cnt=0; cnt<12*year; cnt++)
	{
		f_p_x = f_p*pow(1+rate_m, cnt);
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
	
	sprintf(buff, "全部还款: %.02f, 本金: %.02f, 利息: %.02f\r\n", f_a_sum, f_p_sum, f_i_sum);
	fwrite(buff, 1, strlen(buff), fp);
	printf("%s", buff);
	
	fclose(fp);
}

/// 等额本金
void loan_debj(unsigned int sum, unsigned int year, float rate_y)
{
	unsigned int cnt;
	float		 rate_m = rate_y/12;
	float		 f_p, f_p_x, f_p_sum;
	float		 f_i_x, f_i_sum;
	float		 f_a_x, f_a_sum;
	float		 f_b_x;
	float		 f_tmp;
	FILE*		 fp;
	char		 buff[128];
	
	fp = fopen("debj.txt", "wb+");
	if(NULL == fp)
	{
		printf("open file <debj.txt> fail\r\n");
		return;
	}
	
	sprintf(buff, "等额本金借款[%d 元] [%d 年] [利率%.02f%%]\r\n", sum, year, rate_y*100);
	fwrite(buff, 1, strlen(buff), fp);
	printf("%s", buff);
	
	sprintf(buff, "分期   月供      本金      利息\r\n", sum, year, rate_y*100);
	fwrite(buff, 1, strlen(buff), fp);
	printf("%s", buff);
	
	f_p = sum/(12.0*year);
	
	f_b_x = sum;
	f_p_sum = 0;
	f_i_sum = 0;
	f_a_sum = 0;
	for(cnt=0; cnt<12*year; cnt++)
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
	
	sprintf(buff, "全部还款: %.02f, 本金: %.02f, 利息: %.02f\r\n", f_a_sum, f_p_sum, f_i_sum);
	fwrite(buff, 1, strlen(buff), fp);
	printf("%s", buff);
	
	fclose(fp);
}

int main(void)
{
	FILE* fp;
	unsigned int sum;
	unsigned int year;
	float        rate;
	char		 buff[128];
	char		 tmp[128];
	int			 tmp_index=0, flag=0;
	int 		 len, cnt;
	
	fp = fopen("cfg.txt", "r");
	if(NULL == fp)
	{
		printf("open <cfg.txt> fail\r\n");
	}
	
	len = fread(buff, 1, 127, fp);
	buff[len] = 0;
	
	if(len > 0)
	{
		for(cnt=0; cnt<len+1; cnt++)
		{
			if('\r'==buff[cnt] || '\n'==buff[cnt] || '\0' == buff[cnt])
			{
				tmp[tmp_index] = 0;
				switch(flag)
				{
					case 0:
						sum = atoi(tmp);
						flag = 1;
						break;
					case 1:
						year = atoi(tmp);
						flag = 2;
						break;
					case 2:
						rate = atof(tmp);
						flag = 3;
						break;
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
			printf("sum: %d, year: %d, rate: %f\r\n", sum, year, rate);
		}
	}
	fclose(fp);
	
	loan_debx(sum, year, rate);
	loan_debj(sum, year, rate);
}

