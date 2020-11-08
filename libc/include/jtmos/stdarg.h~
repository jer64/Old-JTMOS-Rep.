/* Gemini Nucleus
 *
 * $Id: stdarg.h,v 1.1.1.1 2000/03/24 17:46:12 verdich Exp $
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA. 
 */

#ifndef __STDARG_H__
#define __STDARG_H__

typedef char *va_list[1];

#define va_start(ap,pn) ((ap)[0]=(char *)&pn+\
                        ((sizeof(pn)+sizeof(int)-1)&~(sizeof(int)-1)),(void)0)
#define va_arg(ap,type) ((ap)[0]+=((sizeof(type)+sizeof(int)-1)&\
                        ~(sizeof(int)-1)),(*(type *)((ap)[0]-((sizeof(type)+\
                        sizeof(int)-1)&~(sizeof(int)-1)))))
#define va_end(ap) ((ap)[0]=0,(void)0)

#endif /* ifndef __STDARG_H__ */

