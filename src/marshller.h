
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

/* BOOLEAN:NONE (marshller.define:5) */
extern void g_marshal_BOOLEAN__VOID (GClosure     *closure,
                                     GValue       *return_value,
                                     guint         n_param_values,
                                     const GValue *param_values,
                                     gpointer      invocation_hint,
                                     gpointer      marshal_data);
#define g_marshal_BOOLEAN__NONE	g_marshal_BOOLEAN__VOID

/* BOOLEAN:OBJECT (marshller.define:6) */
extern void g_marshal_BOOLEAN__OBJECT (GClosure     *closure,
                                       GValue       *return_value,
                                       guint         n_param_values,
                                       const GValue *param_values,
                                       gpointer      invocation_hint,
                                       gpointer      marshal_data);

/* VOID:BOOLEAN (marshller.define:7) */
#define g_marshal_VOID__BOOLEAN	g_cclosure_marshal_VOID__BOOLEAN

/* VOID:BOOLEAN,POINTER (marshller.define:8) */
extern void g_marshal_VOID__BOOLEAN_POINTER (GClosure     *closure,
                                             GValue       *return_value,
                                             guint         n_param_values,
                                             const GValue *param_values,
                                             gpointer      invocation_hint,
                                             gpointer      marshal_data);

/* VOID:BOXED,BOXED (marshller.define:9) */
extern void g_marshal_VOID__BOXED_BOXED (GClosure     *closure,
                                         GValue       *return_value,
                                         guint         n_param_values,
                                         const GValue *param_values,
                                         gpointer      invocation_hint,
                                         gpointer      marshal_data);

/* VOID:OBJECT (marshller.define:10) */
#define g_marshal_VOID__OBJECT	g_cclosure_marshal_VOID__OBJECT

/* VOID:POINTER (marshller.define:11) */
#define g_marshal_VOID__POINTER	g_cclosure_marshal_VOID__POINTER

/* VOID:STRING,BOXED,FLAGS (marshller.define:12) */
extern void g_marshal_VOID__STRING_BOXED_FLAGS (GClosure     *closure,
                                                GValue       *return_value,
                                                guint         n_param_values,
                                                const GValue *param_values,
                                                gpointer      invocation_hint,
                                                gpointer      marshal_data);

/* VOID:STRING,BOXED,INT,BOOLEAN (marshller.define:13) */
extern void g_marshal_VOID__STRING_BOXED_INT_BOOLEAN (GClosure     *closure,
                                                      GValue       *return_value,
                                                      guint         n_param_values,
                                                      const GValue *param_values,
                                                      gpointer      invocation_hint,
                                                      gpointer      marshal_data);

/* VOID:UINT,POINTER (marshller.define:14) */
#define g_marshal_VOID__UINT_POINTER	g_cclosure_marshal_VOID__UINT_POINTER

/* VOID:UINT64,UINT64 (marshller.define:15) */
extern void g_marshal_VOID__UINT64_UINT64 (GClosure     *closure,
                                           GValue       *return_value,
                                           guint         n_param_values,
                                           const GValue *param_values,
                                           gpointer      invocation_hint,
                                           gpointer      marshal_data);

/* VOID:VOID (marshller.define:16) */

/* VOID:INT,INT (marshller.define:17) */
extern void g_marshal_VOID__INT_INT (GClosure     *closure,
                                     GValue       *return_value,
                                     guint         n_param_values,
                                     const GValue *param_values,
                                     gpointer      invocation_hint,
                                     gpointer      marshal_data);

G_END_DECLS

#endif /* __g_marshal_MARSHAL_H__ */

