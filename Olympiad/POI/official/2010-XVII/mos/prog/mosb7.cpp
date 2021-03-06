/************************************************************************* 
 *                                                                       *
 *                    XVII Olimpiada Informatyczna                       *
 *                                                                       *
 *   Zadanie:           Mosty (MOS)                                      *
 *   Plik:              mosb7.cpp                                        * 
 *   Autor:             Bartosz Gorski                                   * 
 *   Opis:              Rozwiazanie niepoprawne. Wyszukuje binarnie      * 
 *                      po wyniku, ale heurystycznie sprawdza istnienie  * 
 *                      cyklu Eulera. Dla kazdego wierzcholka dzieli     *   
 *                      jego zbior krawedzi na dwa zbiory skierowanych   * 
 *                      i nieskierowanych. Znajdujac cykl najpierw       * 
 *                      korzysta z krawedzi skierowanych a potem         *
 *                      z nieskierowanych. Biorac krawedz z danego       *
 *                      zbioru wybiera ta, ktora prowadzi do wierzcholka,*
 *                      ktory posiada najliczniejszy zbior krawedzi      *
 *                      skierowanych i ktorej przejscie zachowuje        *
 *                      warunek istnienia cyklu Eulera.                  *
 *                      Rozwiazanie wykorzystuje STLa.                   *
 *   Zlozonosc czasowa: O((n + m) * log MAX_WIATR)                       * 
 *                                                                       * 
 *************************************************************************/

#include<cstdio>
#include<cstdlib>
#include<algorithm>
#include<vector>
#include<list>
using namespace std;

#define MINN(a,b) ((a)>(b)?(b):(a))
#define MAXX(a,b) ((a)<(b)?(b):(a))

#define MAX_N 1000
#define MAX_M 2000

struct edge
{
	int a,b,c,nr;
	edge(){}
	edge(int aa,int bb,int cc,int nnr)
	{
		a = aa; b = bb; c = cc; nr = nnr;
	}
};

vector<edge> G[MAX_N], GR[MAX_N], GW[MAX_N];
edge E[2*MAX_M];

int n,m,a,b,c,c1,c2,l,r,degIn[MAX_N],degOut[MAX_N],degNor[MAX_N];
bool u[MAX_M+1];
list<edge> eulerWay;

bool correctDeg(int in, int out, int nor)
{
	return ((in+out+nor)%2==0) && (in<=(out+nor)) && (out<=(in+nor));
}

void euler(edge a)
{
	while(!GW[a.b].empty())
	{
		int deg=-1,ver=0;
		for(int i=0; i<(int)GW[a.b].size(); i++)
			if(deg<(int)GW[GW[a.b][i].b].size())
			{
				ver=i;
				deg=GW[GW[a.b][i].b].size();
			}
		swap(GW[a.b].back(),GW[a.b][ver]);
		edge b = GW[a.b].back();
		GW[a.b].pop_back();
		if(!u[b.nr])
		{
			degOut[b.a]--;
			degIn[b.b]--;
			u[b.nr]=true;
			euler(b);
		}
	}
	while(!GR[a.b].empty())
	{
		int deg=-1,ver=0;
		for(int i=0; i<(int)GR[a.b].size(); i++)
		{
			int v = GR[a.b][i].b;
			if(((degOut[v]>0) && correctDeg(degIn[v],degOut[v]-1,degNor[v]-1))
    			|| ((degNor[v]>1) && correctDeg(degIn[v],degOut[v],degNor[v]-2))
                || (v==0 && (degIn[v]+degOut[v]+degNor[v]-1)==0))
				if(deg<(int)GW[GR[a.b][i].b].size())
				{
					ver=i;
					deg=GW[GR[a.b][i].b].size();
				}
		}
		swap(GR[a.b].back(),GR[a.b][ver]);	
		edge b = GR[a.b].back();
		GR[a.b].pop_back();
		if(!u[b.nr])
		{
			degNor[b.a]--;
			degNor[b.b]--;
			u[b.nr]=true;
			euler(b);
		}
	}
	eulerWay.push_front(a);
}

bool existEulerCycle(int c)
{
	for(int i=0; i<n; i++)
	{
		GR[i].clear();
		GW[i].clear();
		degIn[i]=0;
		degOut[i]=0;
		degNor[i]=0;
	}
	for(int i=0; i<m; i++)
	{
		u[i]=false;
		if(E[2*i].c>c)
		{
			GW[E[2*i+1].a].push_back(E[2*i+1]);
			degIn[E[2*i+1].b]++;
			degOut[E[2*i+1].a]++;
		}
		else
			if(E[2*i+1].c>c)
			{
				GW[E[2*i].a].push_back(E[2*i]);
				degIn[E[2*i].b]++;
				degOut[E[2*i].a]++;
			}
			else
			{
				GR[E[2*i].a].push_back(E[2*i]);
				GR[E[2*i+1].a].push_back(E[2*i+1]);
				degNor[E[2*i].a]++;
				degNor[E[2*i+1].a]++;
			}
	}
	for(int i=0; i<n; i++)
		if(!correctDeg(degIn[i],degOut[i],degNor[i]))
			return false;
	eulerWay.clear();
	euler(edge(0,0,0,m));
	eulerWay.pop_front();
	int a=0,b=0;
	for(list<edge>::iterator il = eulerWay.begin(); il != eulerWay.end(); il++)
	{
		if(a!=il->a)
			return false;
		a=il->b;
		b++;
	}
	return b==m && a==0;
}

int main()
{
	scanf("%d%d",&n,&m);
	for(int i=0; i<n; i++)
		G[i].clear();
	l=r=0;
	for(int i=0; i<m; i++)
	{
		scanf("%d%d%d%d",&a,&b,&c1,&c2);
		a--; b--;
		G[a].push_back(edge(a,b,c1,i));
		E[2*i]=edge(a,b,c1,i);
		G[b].push_back(edge(b,a,c2,i));
		E[2*i+1]=edge(b,a,c2,i);
		l=MAXX(l,MINN(c1,c2));
		r=MAXX(r,MAXX(c1,c2));
	}
	for(int i=0; i<n; i++)
		if(G[i].size()%2!=0)
		{
			printf("NIE\n");
			return 0;
		}
	while(l<r)
	{
		c=(l+r)/2;
		if(existEulerCycle(c))
			r=c;
		else
			l=c+1;
	}
	if(!existEulerCycle(l))
	{
		printf("NIE\n");
		return 0;
	}
	printf("%d\n",l);
	for(list<edge>::iterator il = eulerWay.begin(); il != eulerWay.end(); il++)
		printf("%d ",il->nr+1);
	printf("\n");
	return 0;
}
