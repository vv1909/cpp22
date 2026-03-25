#include <iostream>
using namespace std;

int mat[64]; 
int vmat[64] =
{
	2, 3, 4, 4, 4, 4, 3, 2,
	3, 4, 6, 6, 6, 6, 4, 3,
	4, 6, 8, 8, 8, 8, 6, 4,
	4, 6, 8, 8, 8, 8, 6, 4,
	4, 6, 8, 8, 8, 8, 6, 4,
	4, 6, 8, 8, 8, 8, 6, 4,
	3, 4, 6, 6, 6, 6, 4, 3,
	2, 3, 4, 4, 4, 4, 3, 2
};

void print_mat(int *mat){
	int i;

	cout<<"OUTPUT:\n";
	i = 0;
	for(; i < 64; i++){
		cout<<mat[i]<<"  ";
		if (mat[i]<10)
			cout<<" ";
		if ((i+1)%8==0)
			cout<<"\n\n";
	}
}

void decr_vmat(int *pos_arr, int pos, int *count){
	vmat[pos]--;
	pos_arr[*count] = pos;
	(*count)++;
}

void sort(int *pos_pos, int count)
{
	int i, a, j = 1;

	while (j){
		i = 1;
		j = 0;
		while (i < count){
			if (vmat[pos_pos[i-1]] > vmat[pos_pos[i]]){
				a = pos_pos[i];
				pos_pos[i] = pos_pos[i-1];
				pos_pos[i-1] = a;
				j = 1;
			}
			i++;
		}
	}
}

int gen_pos(int *pos_pos, int pos){
	int count = 0;
	int row = pos / 8;
	int col = pos % 8;

	if (col > 0 && row > 1 && !mat[pos-17]) decr_vmat(pos_pos, pos-17, &count); // up-leftd
	if (col < 7 && row > 1 && !mat[pos-15]) decr_vmat(pos_pos, pos-15, &count); // up-rightl
	if (col > 0 && row < 6 && !mat[pos+15]) decr_vmat(pos_pos, pos+15, &count); // down-lefti
	if (col < 7 && row < 6 && !mat[pos+17]) decr_vmat(pos_pos, pos+17, &count); // down-rightr
	if (col > 1 && row > 0 && !mat[pos-10]) decr_vmat(pos_pos, pos-10, &count); // left-upv
	if (col > 1 && row < 7 && !mat[pos+6])  decr_vmat(pos_pos, pos+6, &count);  // left-downi
	if (col < 6 && row > 0 && !mat[pos-6])  decr_vmat(pos_pos, pos-6, &count);  // right-upa
	if (col < 6 && row < 7 && !mat[pos+10]) decr_vmat(pos_pos, pos+10, &count); // right-downm

	sort(pos_pos, count);
	return count;
}

int process(int pos, int count){
	int pos_pos[8], i, gen_amt;

	if (mat[pos])
		return 0;
	mat[pos] = count;
	gen_amt = gen_pos(pos_pos, pos);

	i = 0;
	while(i < gen_amt){
		if (process(pos_pos[i], count+1)) return 1;
		i++;
	}
 
	if (count == 64)
		return 1;

	i = 0;
	for (; i < gen_amt; i++){
		vmat[pos_pos[i]]++;
	}
	mat[pos] = 0;
	return 0;
}

int main(){
	int i = 0, pos;

	for (; i < 64; i++) mat[i] = 0;
	cout<<"Input initial position: ";
	cin>>pos;
	while (pos < 1 || pos > 64){
		cout<<"Invalid starting position";
		cin>>pos;
	}
	pos--;
	if (!process(pos, 1))
		cout << "Smth went wrong";
	else
		print_mat(mat);

	return 0;
}