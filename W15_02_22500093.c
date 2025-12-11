#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char GRADE[3][10]={"Deluxe", "Luxury", "Suite"}; // 객실등급 명칭 

struct st_card{		// 객실 관리 카드 레코드
  int no;		// 객실번호 (3자리 숫자)
  int grade; // 객실 등급 (1:deluxe, 2:luxury, 3:suite)
  int capa;		// 수용최대인원 (2~10명)
  char name[20];	// 예약자 성명				
  int person;		// 예약 인원
};

int loadCards(struct st_card* p[]);		// 객실정보 읽기
void listCards(struct st_card* p[], int n);	// 객실 목록 출력
void makeReserve(struct st_card* p[], int n);		// 객실 예약
void cancelReserve(struct st_card* p[], int n);	// 예약 취소
void findCards(struct st_card* p[], int n);	// 객실 검색
void saveReport(struct st_card* p[], int n);	// 보고서 저장
void findRoom(struct st_card* p[], int n);	// 빈 객실 찾기
int addCards(struct st_card* p[], int n);	// 객실 추가 

int main(){
  struct st_card* cards[50]; 	// 최대 50개의 객실을 관리할 수 있는 구조체 포인터 배열
  int count;	// 현재 등록된 카드 수
  int menu;	// 메뉴번호
  count = loadCards(cards);	// 시작 전에 파일로부터 객실 리스트를 모두 읽어온다.
  while(1)
  {
    // 메뉴 출력
    printf("\n[1]객실목록 [2]예약 [3]예약취소 [4]검색 [5]저장 [6]빈객실찾기 [7]객실추가 [0]종료 >> ");
    scanf("%d",&menu);	// 메뉴를 읽음

    if(menu==1){
      listCards(cards, count);	// 객실 목록 출력
    }
    else if(menu==2){
      makeReserve(cards, count);	// 예약하기
    }
    else if(menu==3){
      cancelReserve(cards, count); // 예약 취소
    }
    else if(menu==4){
      findCards(cards, count);	// 검색
    }
    else if(menu==5){
      saveReport(cards, count);	// 보고서 저장
    }
    else if(menu==6){
      findRoom(cards, count);	// 빈 객실 찾기
    }
    else if(menu==7){
      count = addCards(cards, count);	// 객실 추가
    }
    else {
      break;
    }
  }
}

int loadCards(struct st_card* p[]){
  FILE *file;
  int n=0;
  file = fopen("rooms.txt","r");
  while(!feof(file)){
    p[n]=(struct st_card*)malloc(sizeof(struct st_card));
    int result = fscanf(file,"%d %d %d",&(p[n]->no),&(p[n]->grade),&(p[n]->capa));
    if (result < 1) break;
    p[n]->person = 0;
    n++;
  }
  fclose(file);
  printf("%d개의 객실 정보를 읽었습니다.\n",n);
  return n;
}

void listCards(struct st_card * p[], int n){
  int i;
  printf("\n전체 객실 목록입니다.\n");
  printf("객실번호 / 등급 / 최대인원 / 예약상황\n");

  for(i=0; i<n; i++){
    printf("%d호 / %s / %d명 / ", p[i]->no, GRADE[p[i]->grade-1], p[i]->capa);
    if(p[i]->person > 0)
      printf("예약 : %s %d명\n",p[i]->name, p[i]->person);
    else
      printf("예약없음\n");
  }
}

// 15-5
/*  문제없이 예약이 완료될 때까지 객실번호를 다시 입력받도록 한다.
객실번호가 현재 등록된 객실에 없는 경우에 "예약불가(객실번호 오류)" 에러 표시
이미 다른 사람에 의해 예약된 객실인 경우에 "예약불가(예약된 객실)" 에러 표시
해당 객실의 최대 수용인원보다 많은 인원으로 예약하는 경우 "예약불가(인원 초과)" 에러 표시    */
void makeReserve(struct st_card* p[], int n){
  char name[20];
  struct st_card* rp; // 예약할 카드레코드 구조체의 포인터값
  int i, roomno, newperson;

  printf("예약하실 고객 이름은? >> ");
  scanf("%s", name);
  printf("예약 인원수를 입력하시오. >> ");
  scanf("%d", &newperson); 


  while(1){
  printf("예약하실 객실 번호는? >> ");
  scanf("%d", &roomno);


  for(i=0; i<n; i++){
    if(p[i]->no == roomno) break;
  }
  if(i==n){
    printf("예약불가(객실번호 오류)\n");
    continue;
  } 

    if(p[i]->person > 0){
        printf("예약불가(예약된 객실)\n");
        continue;
    }

    if(p[i]->capa < newperson){
        printf("예약불가(인원 초과)\n");
        continue;
    }

    break;

  }
  for(i=0;i<n;i++){
    if(p[i]->no==roomno){
      rp=p[i];
      break;
    }
  }
  rp->person = newperson;
  strcpy(rp->name, name);
  printf("예약완료!\n");
}

// 15-6
void cancelReserve(struct st_card* p[], int n){
    char name[20];
  struct st_card* rp; // 예약할 카드레코드 구조체의 포인터값
  int i, roomno, new;

    printf("예약 취소하실 객실 번호는? >> ");
    scanf("%d", &roomno);

for(i=0;i<n;i++){
    if(p[i]->no==roomno){
      rp=p[i];
      break;
    }
  }

    if(rp->person == 0){
        printf("예약된 객실이 아닙니다.\n");
    }
    else {
        printf("객실번호 / 등급 / 예약자 / 예약인원\n");
        printf("%d호 / %s / %s / %d명\n", rp->no, GRADE[rp->grade-1], rp->name, rp->person);
        printf("위 예약을 취소할까요? (YES 1, NO 0) >> ");
        scanf("%d", &new);

        rp->person = 0;
        rp->name[0] = '\0';

        if(new == 1) printf("취소했습니다.\n");
    }
}

// 15-7
void findCards(struct st_card * p[], int n){

int i, count=0;
  char name[20];
  while(1){
    printf("검색하려는 고객 이름을 2글자 이상 입력하세요. >> ");
    scanf("%s", name);
    if(strlen(name) > 1) break;
  }

printf("객실번호 / 등급 / 예약자 / 예약인원 / 최대인원\n");

  for(i=0; i<n; i++){
        if( strstr(p[i]->name, name) != 0 ){
            count ++;
            printf("%d호 / %s / %s / %d명 / %d명\n", p[i]->no, GRADE[p[i]->grade -1], p[i]->name, p[i]->person, p[i]->capa);
        }
    }
    printf("%d명 검색됨\n", count);
}

// 15-8
void saveReport(struct st_card* p[], int n){
    FILE* file1;
    file1 = fopen("rooms_report.txt","wt");
  
    int i;

    fprintf(file1, "객실 현황 보고서\n");

    int room_c=0;
    int p_c=0;
    int all=0;

for(i=0; i<n; i++){
    if(p[i]->grade == 1){
        if(p[i]->person > 0){
            room_c ++;
            p_c += p[i]->person;

        }
        else all += p[i]->capa;
    }
}

fprintf(file1, "\n객실등급 : Deluxe\n");
fprintf(file1, "1. 객실 수 : 총 3실 중 %d실 예약 중\n", room_c);
fprintf(file1, "2. 예약 고객 : 총 %d명\n", p_c);
fprintf(file1, "3. 남은 최대수용 고객 : 총 %d명\n", all);


 room_c=0;
 p_c=0;
all=0;

for(i=0; i<n; i++){
    if(p[i]->grade == 2){
        if(p[i]->person > 0){
            room_c ++;
            p_c += p[i]->person;

        }
        else all += p[i]->capa;
    }
}

fprintf(file1, "\n객실등급 : Luxury\n");
fprintf(file1, "1. 객실 수 : 총 8실 중 %d실 예약 중\n", room_c);
fprintf(file1, "2. 예약 고객 : 총 %d명\n", p_c);
fprintf(file1, "3. 남은 최대수용 고객 : 총 %d명\n", all);


room_c=0;
   p_c=0;
  all=0;

    
for(i=0; i<n; i++){
    if(p[i]->grade == 3){
        if(p[i]->person > 0){
            room_c ++;
            p_c += p[i]->person;

        }
        else all += p[i]->capa;
    }
}
fprintf(file1, "\n객실등급 : Suite\n");
fprintf(file1, "1. 객실 수 : 총 3실 중 %d실 예약 중\n", room_c);
fprintf(file1, "2. 예약 고객 : 총 %d명\n", p_c);
fprintf(file1, "3. 남은 최대수용 고객 : 총 %d명\n", all);


fprintf(file1, "\n객실 예약 목록\n");
fprintf(file1, "객실번호 / 등급 / 최대인원 / 예약상황\n");

p_c = 0;
for(i=0; i<n; i++){
    if(p[i]->person > 0){
        fprintf(file1, "%d호 / %s / %d명 / 예약 : %s %d명\n", p[i]->no, GRADE[p[i]->grade -1], p[i]->capa, p[i]->name, p[i]->person);
        p_c += p[i]->person;
    }
    else fprintf(file1, "%d호 / %s / %d명 / 예약없음\n", p[i]->no, GRADE[p[i]->grade -1], p[i]->capa);
}

fprintf(file1, "예약 인원 합계 : %d명\n", p_c);

fclose(file1);


printf("저장되었습니다.\n");

}	

//15-9
void findRoom(struct st_card* p[], int n){

    int i;
    int person;
    int g;
    int room_c=0;

printf("투숙 인원수는? >> ");
    scanf("%d", &person);

printf("원하시는 객실 등급은? (Deluxe 1, Luxury 2. Suite 3, 무관 0) >> ");
    scanf("%d", &g);

    printf("객실번호 / 등급 / 최대인원\n");

  for(i=0; i<n; i++){
    if(p[i]->person == 0 && (p[i]->capa > person)){
      if(g == 1 && (p[i]->grade == 1)){
        printf("%d호 / %s / %d명\n", p[i]->no, GRADE[p[i]->grade -1], p[i]->capa);
        room_c++;
      }
      else if(g == 2 && (p[i]->grade == 2) ){
        printf("%d호 / %s / %d명\n", p[i]->no, GRADE[p[i]->grade -1], p[i]->capa);
        room_c++;
      }
      else if(g == 3 && (p[i]->grade == 3) ){
        printf("%d호 / %s / %d명\n", p[i]->no, GRADE[p[i]->grade -1], p[i]->capa);
        room_c++;
      }
      else if(g == 0){
        printf("%d호 / %s / %d명\n", p[i]->no, GRADE[p[i]->grade -1], p[i]->capa);
        room_c++;
      }
    }
  }
  printf("총 %d개의 객실이 예약 가능합니다.\n", room_c);
}



// 15-10
int addCards(struct st_card* p[], int n){

  int newroom;
  int newgrade;
  int newpeople;
  int i;
  

while(1){

printf("추가할 객실번호는? >> ");
int conti=0;
      scanf("%d", &newroom);

  for(i=0; i<n; i++){
    if(p[i]->no == newroom){  
      printf("이미 등록된 객실번호입니다.\n");
      conti =1;
      break;
    }
  }
  if(conti) continue;

  int corr=0;

   for(i=0; i<n; i++){
    if(p[i]->no +1 == newroom){  
      corr =1;
    }
  }
  if(corr==0){
    printf("연속적인 객실번호가 아닙니다.\n");
    continue;
  } 

  break;
}

printf("객실등급(Deluxe 1, Luxury 2. Suite 3)과 최대인원을 입력하세요 >> ");
    scanf("%d %d", &newgrade, &newpeople);


    FILE* file1;
    file1 = fopen("rooms.txt","at");

fprintf(file1, "%d %d %d", newroom, newgrade, newpeople);
fclose(file1);

printf("객실을 추가하였습니다.\n");

  return n+1;
}