o p t i o n s   {  
 	 l a n g u a g e = " C p p " ;  
 }  
  
 c l a s s   C a l c P a r s e r   e x t e n d s   P a r s e r ;  
 o p t i o n s   {  
 	 g e n H a s h L i n e s   =   t r u e ; 	 	 / /   i n c l u d e   l i n e   n u m b e r   i n f o r m a t i o n  
 	 b u i l d A S T   =   t r u e ; 	 	 	 / /   u s e s   C o m m o n A S T   b y   d e f a u l t  
 }  
  
 e x p r  
 	 : 	 m e x p r   ( P L U S ^   m e x p r ) *   S E M I !  
 	 ;  
  
 m e x p r  
 	 : 	 a t o m   ( S T A R ^   a t o m ) *  
 	 ;  
  
 a t o m : 	 I N T  
 	 ;  
  
 c l a s s   C a l c L e x e r   e x t e n d s   L e x e r ;  
  
 W S _ 	 : 	 ( '   '  
 	 | 	 ' \ t '  
 	 | 	 ' \ n '  
 	 | 	 ' \ r ' )  
 	 	 {   _ t t y p e   =   A N T L R _ U S E _ N A M E S P A C E ( a n t l r ) T o k e n : : S K I P ;   }  
 	 ;  
  
 L P A R E N : 	 ' ( '  
 	 ;  
  
 R P A R E N : 	 ' ) '  
 	 ;  
  
 S T A R : 	 ' * '  
 	 ;  
  
 P L U S : 	 ' + '  
 	 ;  
  
 S E M I : 	 ' ; '  
 	 ;  
  
 p r o t e c t e d  
 D I G I T  
 	 : 	 ' 0 ' . . ' 9 '  
 	 ;  
  
 I N T 	 : 	 ( D I G I T ) +  
 	 ;  
  
 c l a s s   C a l c T r e e W a l k e r   e x t e n d s   T r e e P a r s e r ;  
  
 e x p r   r e t u r n s   [ f l o a t   r ]  
 {  
 	 f l o a t   a , b ;  
 	 r = 0 ;  
 }  
 	 : 	 # ( P L U S   a = e x p r   b = e x p r ) 	 { r   =   a + b ; }  
 	 | 	 # ( S T A R   a = e x p r   b = e x p r ) 	 { r   =   a * b ; }  
 	 | 	 i : I N T 	 	 	 { r   =   a t o f ( i - > g e t T e x t ( ) . c _ s t r ( ) ) ; }  
 	 ;  
 