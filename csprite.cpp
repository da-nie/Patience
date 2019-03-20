//====================================================================================================
//подключаемые библиотеки
//====================================================================================================
#include "csprite.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "tga.h"

//====================================================================================================
//конструктор и деструктор класса
//====================================================================================================

//----------------------------------------------------------------------------------------------------
//конструктор
//----------------------------------------------------------------------------------------------------
CSprite::CSprite(void)
{
 Width=0;
 Height=0;
 Data=NULL;
}
//----------------------------------------------------------------------------------------------------
//деструктор
//----------------------------------------------------------------------------------------------------
CSprite::~CSprite()
{
 Release();
}

//====================================================================================================
//статические функции класса
//====================================================================================================

//----------------------------------------------------------------------------------------------------
//перекодировка цвета
//----------------------------------------------------------------------------------------------------
uint32_t CSprite::ConvertColor(COLOR_MODE color_mode,uint32_t color)
{
 uint8_t ir=0;
 uint8_t ig=0;
 uint8_t ib=0;
 if (color_mode==COLOR_MODE_RGB)
 {
  ir=(color>>16)&0xFF;
  ig=(color>>8)&0xFF;
  ib=(color)&0xFF;
 }
 if (color_mode==COLOR_MODE_BGR)
 {
  ib=(color>>16)&0xFF;
  ig=(color>>8)&0xFF;
  ir=(color)&0xFF;
 }
 if (color_mode==COLOR_MODE_GBR)
 {
  ig=(color>>16)&0xFF;
  ig=(color>>8)&0xFF;
  ir=(color)&0xFF;
 }
 if (color_mode==COLOR_MODE_GRB)
 {
  ig=(color>>16)&0xFF;
  ir=(color>>8)&0xFF;
  ib=(color)&0xFF;
 }
 if (color_mode==COLOR_MODE_RBG)
 {
  ir=(color>>16)&0xFF;
  ib=(color>>8)&0xFF;
  ig=(color)&0xFF;
 }
 if (color_mode==COLOR_MODE_BRG)
 {
  ib=(color>>16)&0xFF;
  ir=(color>>8)&0xFF;
  ig=(color)&0xFF;
 }

 color=ir;
 color<<=8;
 color|=ig;
 color<<=8;
 color|=ib;

 return(color);
}
//----------------------------------------------------------------------------------------------------
//прямое отображение цвета
//----------------------------------------------------------------------------------------------------
uint32_t CSprite::ForwardColor(COLOR_MODE color_mode,uint32_t color)
{
 return(color);
}

//====================================================================================================
//закрытые функции класса
//====================================================================================================

//====================================================================================================
//открытые функции класса
//====================================================================================================

//----------------------------------------------------------------------------------------------------
//загрузить спрайт из файла
//----------------------------------------------------------------------------------------------------
bool CSprite::Load(const char *file_name)
{
 Release();
 Data=reinterpret_cast<uint32_t*>(LoadTGAFromFile(file_name,Width,Height));
 if (Data==NULL) return(false);
 //изменяем альфа-канал
 uint32_t *ptr=Data;
 int32_t n;
 int32_t length=Width*Height;
 uint32_t alpha_mask=0xFF;
 alpha_mask<<=24;
 for(n=0;n<length;n++)
 {
  uint32_t color=*ptr;
  *ptr=(color&0x00FFFFFF)|alpha_mask;
  ptr++;
 }
 return(true);
}
//----------------------------------------------------------------------------------------------------
//загрузить спрайт из ресурсов
//----------------------------------------------------------------------------------------------------
bool CSprite::Load(HMODULE hModule,int32_t id)
{
 Release();
 Data=reinterpret_cast<uint32_t*>(LoadTGAFromResource(hModule,id,Width,Height));
 if (Data==NULL) return(false);
 //изменяем альфа-канал
 uint32_t *ptr=Data;
 int32_t n;
 int32_t length=Width*Height;
 uint32_t alpha_mask=0xFF;
 alpha_mask<<=24;
 for(n=0;n<length;n++)
 {
  uint32_t color=*ptr;
  *ptr=(color&0x00FFFFFF)|alpha_mask;
  ptr++;
 }
 return(true);
}

//----------------------------------------------------------------------------------------------------
//отобразить спрайт
//----------------------------------------------------------------------------------------------------
void CSprite::Put(IVideo *iVideo_Ptr,int32_t x,int32_t y,bool alpha,color_func_ptr_t color_func_ptr,COLOR_MODE color_mode) const
{
 if (Data==NULL || Width==0 || Height==0) return;//спрайт отсутствует

 uint32_t *vptr;
 uint32_t linesize;
 iVideo_Ptr->GetVideoPointer(vptr,linesize);
 uint32_t screen_width;
 uint32_t screen_height;
 iVideo_Ptr->GetScreenSize(screen_width,screen_height);  

 int32_t x1=x;
 int32_t x2=x+Width;
 int32_t y1=y;
 int32_t y2=y+Height;
 if (x2<0) return;//невидим
 if (y2<0) return;//невидим
 if (x1>static_cast<int32_t>(screen_width)) return;//невидим
 if (y1>static_cast<int32_t>(screen_height)) return;//невидим
 //обрезаем по границам экрана
 if (x1<0) x1=0;
 if (x2>static_cast<int32_t>(screen_width)) x2=screen_width;
 if (y1<0) y1=0;
 if (y2>static_cast<int32_t>(screen_height)) y2=screen_height;
 int32_t ly,lx;
 uint32_t *s_ptr=Data+((x1-x)+(y1-y)*Width);
 uint32_t *v_ptr=vptr+(x1+y1*linesize);
 int32_t dv_ptr=linesize;
 int32_t ds_ptr=Width;
 if (alpha==false)
 {
  int32_t length=(x2-x1);
  for(ly=y1;ly<y2;ly++,v_ptr+=dv_ptr,s_ptr+=ds_ptr) 
  {
   uint32_t *s_ptr_l=s_ptr;
   uint32_t *v_ptr_l=v_ptr;
   for(lx=0;lx<length;lx++)
   {
    uint32_t color=*s_ptr_l;
	s_ptr_l++;
    *v_ptr_l=(*color_func_ptr)(color_mode,color);
	v_ptr_l++;
   }
   //memcpy(v_ptr,s_ptr,length*sizeof(uint32_t));
  }
 }
 else
 {
  int32_t length=x2-x1;
  for(ly=y1;ly<y2;ly++,v_ptr+=dv_ptr,s_ptr+=ds_ptr)
  {
   uint32_t *s_ptr_l=s_ptr;
   uint32_t *v_ptr_l=v_ptr;
   for(lx=0;lx<length;lx++)
   {
    uint32_t color=*s_ptr_l;
	s_ptr_l++;
	if (color&0xFF000000) *v_ptr_l=(*color_func_ptr)(color_mode,color);
    v_ptr_l++;
   }
  }
 }
}
//----------------------------------------------------------------------------------------------------
//отобразить часть спрайта
//----------------------------------------------------------------------------------------------------
void CSprite::PutSpriteItem(IVideo *iVideo_Ptr,int32_t x,int32_t y,int32_t offsetx,int32_t offsety,int32_t width,int32_t height,bool alpha,color_func_ptr_t color_func_ptr,COLOR_MODE color_mode) const
{
 if (Data==NULL || Width==0 || Height==0) return;//спрайт отсутствует

 uint32_t *vptr;
 uint32_t linesize;
 iVideo_Ptr->GetVideoPointer(vptr,linesize);
 uint32_t screen_width;
 uint32_t screen_height;
 iVideo_Ptr->GetScreenSize(screen_width,screen_height);  

 int32_t x1=x;
 int32_t x2=x+width;
 int32_t y1=y;
 int32_t y2=y+height;
 if (x2<0) return;//не видим
 if (y2<0) return;//не видим
 if (x1>static_cast<int32_t>(screen_width)) return;//не видим
 if (y1>static_cast<int32_t>(screen_height)) return;//не видим
 //обрезаем по границам экрана
 if (x1<0) x1=0;
 if (x2>static_cast<int32_t>(screen_width)) x2=screen_width;
 if (y1<0) y1=0;
 if (y2>static_cast<int32_t>(screen_height)) y2=screen_height;
 int32_t ly,lx;
 uint32_t *s_ptr=Data+((offsetx+(x1-x))+(offsety+(y1-y))*Width);
 uint32_t *v_ptr=vptr+(x1+y1*linesize);
 int32_t dv_ptr=linesize;
 int32_t ds_ptr=Width;
 if (alpha==false)
 {
  int32_t length=(x2-x1);
  for(ly=y1;ly<y2;ly++,v_ptr+=dv_ptr,s_ptr+=ds_ptr) 
  {
   uint32_t *s_ptr_l=s_ptr;
   uint32_t *v_ptr_l=v_ptr;
   for(lx=0;lx<length;lx++)
   {
    uint32_t color=*s_ptr_l;
	s_ptr_l++; 
	*v_ptr_l=(*color_func_ptr)(color_mode,color);
    v_ptr_l++;
   }
   //memcpy(v_ptr,s_ptr,length*sizeof(uint32_t));
  }
 }
 else
 {
  int32_t length=x2-x1;
  for(ly=y1;ly<y2;ly++,v_ptr+=dv_ptr,s_ptr+=ds_ptr)
  {
   uint32_t *s_ptr_l=s_ptr;
   uint32_t *v_ptr_l=v_ptr;
   for(lx=0;lx<length;lx++)
   {
    uint32_t color=*s_ptr_l;
	s_ptr_l++; 
	if (color&0xFF000000) *v_ptr_l=color;
    v_ptr_l++;
   }
  }
 }
}
//----------------------------------------------------------------------------------------------------
//отобразить часть спрайта с маской
//----------------------------------------------------------------------------------------------------
void CSprite::PutSpriteItemMask(IVideo *iVideo_Ptr,int32_t x,int32_t y,int32_t offsetx,int32_t offsety,int32_t width,int32_t height,uint8_t *Mask,color_func_ptr_t color_func_ptr,COLOR_MODE color_mode) const
{
 if (Data==NULL || Width==0 || Height==0 || Mask==NULL) return;//спрайт или маска спрайта отсутствует

 uint32_t *vptr;
 uint32_t linesize;
 iVideo_Ptr->GetVideoPointer(vptr,linesize);
 uint32_t screen_width;
 uint32_t screen_height;
 iVideo_Ptr->GetScreenSize(screen_width,screen_height);  

 int32_t x1=x;
 int32_t x2=x+width;
 int32_t y1=y;
 int32_t y2=y+height;
 if (x2<0) return;//не видим
 if (y2<0) return;//не видим
 if (x1>static_cast<int32_t>(screen_width)) return;//не видим
 if (y1>static_cast<int32_t>(screen_height)) return;//не видим
 //обрезаем по границам экрана
 if (x1<0) x1=0;
 if (x2>static_cast<int32_t>(screen_width)) x2=screen_width;
 if (y1<0) y1=0;
 if (y2>static_cast<int32_t>(screen_height)) y2=screen_height;
 int32_t ly,lx;
 uint8_t *m_ptr=Mask;
 uint32_t *s_ptr=Data+((offsetx+(x1-x))+(offsety+(y1-y))*Width);
 uint32_t *v_ptr=vptr+(x1+y1*linesize);
 int32_t dm_ptr=width;
 int32_t dv_ptr=linesize;
 int32_t ds_ptr=Width;
 int32_t length=x2-x1;
 for(ly=y1;ly<y2;ly++,v_ptr+=dv_ptr,s_ptr+=ds_ptr,m_ptr+=dm_ptr)
 {
  uint8_t *m_ptr_l=m_ptr;
  uint32_t *s_ptr_l=s_ptr;
  uint32_t *v_ptr_l=v_ptr;
  for(lx=0;lx<length;lx++)
  {
   uint32_t color=*s_ptr_l;s_ptr_l++; 
   uint8_t m=*m_ptr_l;m_ptr_l++;
   if (m!=0) *v_ptr_l=(*color_func_ptr)(color_mode,color);
   v_ptr_l++;
  }
 }
}

//----------------------------------------------------------------------------------------------------
//отобразить часть спрайта со смешиванием цветов и изменением последовательности RGB
//----------------------------------------------------------------------------------------------------
void CSprite::PutBlendSpriteItem(IVideo *iVideo_Ptr,int32_t x,int32_t y,int32_t offsetx,int32_t offsety,int32_t width,int32_t height,color_func_ptr_t color_func_ptr,COLOR_MODE color_mode) const
{
 if (Data==NULL || Width==0 || Height==0) return;//спрайт отсутствует

 uint32_t *vptr;
 uint32_t linesize;
 iVideo_Ptr->GetVideoPointer(vptr,linesize);
 uint32_t screen_width;
 uint32_t screen_height;
 iVideo_Ptr->GetScreenSize(screen_width,screen_height);  

 int32_t x1=x;
 int32_t x2=x+width;
 int32_t y1=y;
 int32_t y2=y+height;
 if (x2<0) return;//не видим
 if (y2<0) return;//не видим
 if (x1>static_cast<int32_t>(screen_width)) return;//не видим
 if (y1>static_cast<int32_t>(screen_height)) return;//не видим
 //обрезаем по границам экрана
 if (x1<0) x1=0;
 if (x2>static_cast<int32_t>(screen_width)) x2=screen_width;
 if (y1<0) y1=0;
 if (y2>static_cast<int32_t>(screen_height)) y2=screen_height;
 int32_t ly,lx;
 uint32_t *s_ptr=Data+((offsetx+(x1-x))+(offsety+(y1-y))*Width);
 uint32_t *v_ptr=vptr+(x1+y1*linesize);
 int32_t dv_ptr=linesize;
 int32_t ds_ptr=Width;
 int32_t length=x2-x1;
 uint8_t ir=0;
 uint8_t ig=0;
 uint8_t ib=0;
 for(ly=y1;ly<y2;ly++,v_ptr+=dv_ptr,s_ptr+=ds_ptr)
 {
  uint32_t *s_ptr_l=s_ptr;
  uint32_t *v_ptr_l=v_ptr;
  for(lx=0;lx<length;lx++)
  {
   uint32_t color=*s_ptr_l;s_ptr_l++; 
   uint32_t new_color=(*color_func_ptr)(color_mode,color);

   ir=(new_color>>16)&0xFF;
   ig=(new_color>>8)&0xFF;
   ib=(new_color)&0xFF;

   color=*v_ptr_l;
   uint8_t vb=(color)&0xFF;
   uint8_t vg=(color>>8)&0xFF;
   uint8_t vr=(color>>16)&0xFF;

   float ii=(static_cast<float>(ir)+static_cast<float>(ig)+static_cast<float>(ib))/(255.0f*3);
   float iii=(1.0f-ii);
   uint8_t r=static_cast<uint8_t>((static_cast<float>(vr)*iii+static_cast<float>(ir)*ii));
   uint8_t g=static_cast<uint8_t>((static_cast<float>(vg)*iii+static_cast<float>(ig)*ii));
   uint8_t b=static_cast<uint8_t>((static_cast<float>(vb)*iii+static_cast<float>(ib)*ii));

   color=r;
   color<<=8;
   color|=g;
   color<<=8;
   color|=b;

   *v_ptr_l=color;

   v_ptr_l++;
  }
 }
}

//----------------------------------------------------------------------------------------------------
//установить значение програчности
//----------------------------------------------------------------------------------------------------
void CSprite::SetAlpha(uint8_t alpha,uint8_t r,uint8_t g,uint8_t b)
{
 if (Data==NULL || Width==0 || Height==0) return;//спрайт отсутствует
 uint32_t *ptr=Data;
 int32_t n;
 int32_t length=Width*Height;
 uint32_t alpha_mask=alpha;
 alpha_mask<<=24;
 for(n=0;n<length;n++)
 {
  uint32_t color=*ptr;
  uint8_t bi=static_cast<uint8_t>(color&0xFF);
  uint8_t gi=static_cast<uint8_t>((color>>8)&0xFF);
  uint8_t ri=static_cast<uint8_t>((color>>16)&0xFF);
  if (ri==r && gi==g && bi==b) *ptr=(color&0x00FFFFFF)|alpha_mask;
  ptr++;
 }
}
//----------------------------------------------------------------------------------------------------
//удалить спрайт
//----------------------------------------------------------------------------------------------------
void CSprite::Release(void)
{
 if (Data!=NULL) delete[](Data);
 Data=NULL;
 Width=0;
 Height=0;
}
//----------------------------------------------------------------------------------------------------
//получить ширину спрайта
//----------------------------------------------------------------------------------------------------
int32_t CSprite::GetWidth(void) const
{
 return(Width);
}
//----------------------------------------------------------------------------------------------------
//получить высоту спрайта
//----------------------------------------------------------------------------------------------------
int32_t CSprite::GetHeight(void) const
{
 return(Height);
}
//----------------------------------------------------------------------------------------------------
//нормировать изображение спрайта
//----------------------------------------------------------------------------------------------------
void CSprite::Normalize(void)
{
 int32_t x;
 int32_t y;
 //ищем максимальный цвет, игнорируя альфа-канал
 int32_t max_color=0;
 for(y=0;y<Height;y++)
 {
  for(x=0;x<Width;x++)
  {
   uint32_t color=Data[x+y*Width];
   int32_t alpha=(color>>24)&0xff;
   if (alpha==0) continue;
   int32_t r=(color>>16)&0xff;
   int32_t g=(color>>8)&0xff;
   int32_t b=(color>>0)&0xff;
   if (max_color<r) max_color=r;
   if (max_color<g) max_color=g;
   if (max_color<b) max_color=b;
  }
 }
 if (max_color==0) max_color=1;
 for(y=0;y<Height;y++)
 {
  for(x=0;x<Width;x++)
  {
   uint32_t color=Data[x+y*Width];
   int32_t alpha=(color>>24)&0xff;
   if (alpha==0) continue;
   int32_t r=(color>>16)&0xff;
   int32_t g=(color>>8)&0xff;
   int32_t b=(color>>0)&0xff;
   int32_t new_r=(255*r)/max_color;
   int32_t new_g=(255*g)/max_color;
   int32_t new_b=(255*b)/max_color;
   color=0xff;
   color<<=8;
   color|=new_r;
   color<<=8;
   color|=new_g;
   color<<=8;
   color|=new_b;

   Data[x+y*Width]=color;
  }
 }
}
//----------------------------------------------------------------------------------------------------
//сохранить изображение спрайта
//----------------------------------------------------------------------------------------------------
bool CSprite::Save(const char *file_name) const
{
 return(SaveTGA(file_name,Width,Height,reinterpret_cast<uint8_t*>(Data)));
}
 