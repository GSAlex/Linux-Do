
#ifndef __g_marshal_MARSHAL_H__
#define __g_marshal_MARSHAL_H__

#include	<glib-object.h>

G_BEGIN_DECLS

/* VOID:VOID (marshller.define:1) */
#define g_marshal_VOID__VOID	g_cclosure_marshal_VOID__VOID

/* VOID:STRING (marshller.define:2) */
#define g_marshal_VOID__STRING	g_cclosure_marshal_VOID__STRING

/* BOOL:STRING,POINTER (marshller.define:3) */
extern void g_marshal_BOOLEAN__STRING_POINTER (GClosure     *closure,
                                               GValue       *return_value,
                                               guint         n_param_values,
                                               const GValue *param_values,
                                               gpointer      invocation_hint,
                                               gpointer      marshal_data);
#define g_marshal_BOOL__STRING_POINTER	g_marshal_BOOLEAN__STRING_POINTER

G_END_DECLS

#endif /* __g_marshal_MARSHAL_H__ */

