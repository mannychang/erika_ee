/* ###*B*###
 * ERIKA Enterprise - a tiny RTOS for small microcontrollers
 *
 * Copyright (C) 2006-2010  Simone Mannori, Roberto Bucher
 *
 * This file is part of ERIKA Enterprise.
 *
 * ERIKA Enterprise is free software; you can redistribute it
 * and/or modify it under the terms of the GNU General Public License
 * version 2 as published by the Free Software Foundation, 
 * (with a special exception described below).
 *
 * Linking this code statically or dynamically with other modules is
 * making a combined work based on this code.  Thus, the terms and
 * conditions of the GNU General Public License cover the whole
 * combination.
 *
 * As a special exception, the copyright holders of this library give you
 * permission to link this code with independent modules to produce an
 * executable, regardless of the license terms of these independent
 * modules, and to copy and distribute the resulting executable under
 * terms of your choice, provided that you also meet, for each linked
 * independent module, the terms and conditions of the license of that
 * module.  An independent module is a module which is not derived from
 * or based on this library.  If you modify this code, you may extend
 * this exception to your version of the code, but you are not
 * obligated to do so.  If you do not wish to do so, delete this
 * exception statement from your version.
 *
 * ERIKA Enterprise is distributed in the hope that it will be
 * useful, but WITHOUT ANY WARRANTY; without even the implied warranty
 * of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License version 2 for more details.
 *
 * You should have received a copy of the GNU General Public License
 * version 2 along with ERIKA Enterprise; if not, write to the
 * Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301 USA.
 * ###*E*### */

/** 
	@brief www.scicos.org, www.scicoslab.org
	@author Roberto Bucher, SUPSI- Lugano
	@author Simone Mannori, ScicosLab developer
	@date 2006-2010
*/
 
 
# include "scicos_block4.h"
# include "machine.h"
#include <stdio.h>
#include <math.h>
extern int C2F(dgetrf)();
extern double C2F(dlamch)();
extern double C2F(dlange)();
extern int C2F(dlacpy)();
extern int C2F(dgecon)();
extern int C2F(dgetrs)();
extern int C2F(dgelsy1)();

#if WIN32
#define NULL    0
#endif

#ifndef min
#define min(a,b) ((a) <= (b) ? (a) : (b))
#endif

#ifndef max
#define max(a,b) ((a) >= (b) ? (a) : (b))
#endif

typedef struct
{         int *ipiv;
          int *rank;
          int *jpvt;
          int *iwork;
          double *dwork;
	  double *LAF;
	  double *LBT;
	  double *LAT;
} mat_div_struct ;
void mat_div(scicos_block *block,int flag)
{
 double *u1;
 double *u2;
 double *y;
 int mu1,mu2;
 int nu,nu2;
 int info;
 int i,j,l,lw,lu,ij,ji;
 mat_div_struct *ptr;
 double rcond, ANORM, EPS;

 mu2 =GetInPortRows(block,1);
 nu =GetInPortCols(block,1);
 mu1 =GetInPortRows(block,2);
 nu2 =GetInPortCols(block,2);
 u2=GetRealInPortPtrs(block,1);
 u1=GetRealInPortPtrs(block,2);
 y=GetRealOutPortPtrs(block,1);
 l=max(mu1,nu);
 lu=max(4*nu,min(mu1,nu)+3*mu1+1);
 lw=max(lu,2*min(mu1,nu)+mu2);
             /*init : initialization*/
if (flag==4)
   {if((*(block->work)=(mat_div_struct*) scicos_malloc(sizeof(mat_div_struct)))==NULL)
	{set_block_error(-16);
	 return;}
    ptr=*(block->work);
    if((ptr->ipiv=(int*) scicos_malloc(sizeof(int)*nu))==NULL)
	{set_block_error(-16);
	 scicos_free(ptr);
	 return;}
    if((ptr->rank=(int*) scicos_malloc(sizeof(int)))==NULL)
	{set_block_error(-16);
	 scicos_free(ptr->ipiv);
	 scicos_free(ptr);
	 return;}
    if((ptr->jpvt=(int*) scicos_malloc(sizeof(int)*mu1))==NULL)
	{set_block_error(-16);
	 scicos_free(ptr->rank);
	 scicos_free(ptr->ipiv);
	 scicos_free(ptr);
	 return;}
    if((ptr->iwork=(int*) scicos_malloc(sizeof(int)*nu))==NULL)
	{set_block_error(-16);
	 scicos_free(ptr->jpvt);
	 scicos_free(ptr->rank);
	 scicos_free(ptr->ipiv);
	 scicos_free(ptr);
	 return;}
    if((ptr->dwork=(double*) scicos_malloc(sizeof(double)*lw))==NULL)
	{set_block_error(-16);
	 scicos_free(ptr->iwork);
	 scicos_free(ptr->jpvt);
	 scicos_free(ptr->rank);
	 scicos_free(ptr->ipiv);
	 scicos_free(ptr);
	 return;}
    if((ptr->LAF=(double*) scicos_malloc(sizeof(double)*(nu*mu1)))==NULL)
	{set_block_error(-16);
	 scicos_free(ptr->dwork);
	 scicos_free(ptr->iwork);
	 scicos_free(ptr->jpvt);
	 scicos_free(ptr->rank);
	 scicos_free(ptr->ipiv);
	 scicos_free(ptr);
	 return;}
    if((ptr->LBT=(double*) scicos_malloc(sizeof(double)*(l*mu2)))==NULL)
	{set_block_error(-16);
	 scicos_free(ptr->LAF);
	 scicos_free(ptr->dwork);
	 scicos_free(ptr->iwork);
	 scicos_free(ptr->jpvt);
	 scicos_free(ptr->rank);
	 scicos_free(ptr->ipiv);
	 scicos_free(ptr);
	 return;}
    if((ptr->LAT=(double*) scicos_malloc(sizeof(double)*(nu*mu1)))==NULL)
	{set_block_error(-16);
	 scicos_free(ptr->LBT);
	 scicos_free(ptr->LAF);
	 scicos_free(ptr->dwork);
	 scicos_free(ptr->iwork);
	 scicos_free(ptr->jpvt);
	 scicos_free(ptr->rank);
	 scicos_free(ptr->ipiv);
	 scicos_free(ptr);
	 return;}
   }

       /* Terminaison */
else if (flag==5)
   {ptr=*(block->work);
    if((ptr->LAT)!=NULL) {
   	scicos_free(ptr->ipiv);
   	scicos_free(ptr->rank);
   	scicos_free(ptr->jpvt);
   	scicos_free(ptr->iwork);
   	scicos_free(ptr->LAF);
   	scicos_free(ptr->LBT);
   	scicos_free(ptr->LAT);
   	scicos_free(ptr->dwork);
   	scicos_free(ptr);
   	return;}
   }

else
   {
    ptr=*(block->work);
    EPS=C2F(dlamch)("e",1L);
    ANORM=C2F(dlange)("l",&mu1,&nu,u1,&mu1,ptr->dwork);
    for (j=0;j<mu1;j++)
	{for (i=0;i<nu;i++)
		{ij=i+j*nu;
	 	 ji=j+i*mu1;
		 *(ptr->LAT+ij)=*(u1+ji);}
	}
     for (j=0;j<mu2;j++)
	{for (i=0;i<nu;i++)
		{ij=i+j*l;
	 	 ji=j+i*mu2;
		 *(ptr->LBT+ij)=*(u2+ji);}
	}
     if (mu1==nu)
	{C2F(dlacpy)("F",&nu,&nu,ptr->LAT,&nu,ptr->LAF,&nu);
	 C2F(dgetrf)(&nu,&nu,ptr->LAF,&nu,ptr->ipiv,&info);
	 rcond=0;
 	 if (info==0)
	    {C2F(dgecon)("1",&nu,ptr->LAF,&nu,&ANORM,&rcond,ptr->dwork,ptr->iwork,&info);
	     if (rcond>pow(EPS,0.5))
		{C2F(dgetrs)("N",&nu,&mu2,ptr->LAF,&nu,ptr->ipiv,ptr->LBT,&nu,&info);
		for (j=0;j<nu;j++)
		{for (i=0;i<mu2;i++)
			{ij=i+j*mu2;
			ji=j+i*nu;
			*(y+ij)=*(ptr->LBT+ji);}
		}
		 return;
		}
	    }
	}
    rcond=pow(EPS,0.5);
    for (i=0;i<mu1;i++)    *(ptr->jpvt+i)=0;
    C2F(dgelsy1)(&nu,&mu1,&mu2,ptr->LAT,&nu,ptr->LBT,&l,ptr->jpvt,&rcond,ptr->rank,ptr->dwork,&lw,&info);
    if (info!=0)
	{if (flag!=6)
	    {set_block_error(-7);
             return;
	    }
	}
    for (j=0;j<mu1;j++)
	{for (i=0;i<mu2;i++)
		{ij=i+j*mu2;
		ji=j+i*l;
		*(y+ij)=*(ptr->LBT+ji);}
	}
    }
}
