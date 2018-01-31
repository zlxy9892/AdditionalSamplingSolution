library(ggplot2)

# for raffelson data

df = read.csv('./compare_raf_acc_rf.csv')
df = read.csv('./compare_raf_acc_cart.csv')
df = read.csv('./compare_raf_acc_svm.csv')
df = read.csv('./compare_raf_acc_ipsm.csv')

df$补样数量 = factor(df$补样数量)
colors<-c('#CCCCCC','#5CACEE','#FF6A6A')
ggplot(data=df)+
  geom_boxplot(mapping = aes(x=补样数量, y=推测精度, fill=补样方法))+
  scale_fill_manual(values = colors)+
  labs(x="补样数量", y="推测精度", title = "")+
  theme_bw()+
  theme(plot.title=element_text(hjust=0.5),
        axis.title.x = element_text(size=14, face = 'bold'),
        axis.title.y = element_text(size=14, face = 'bold'),
        legend.position = 'none',
        legend.text = element_text(size = 13, face = 'bold'),
        legend.title = element_blank()
  )+
  coord_cartesian(ylim=c(0,0.8))+
  scale_y_continuous(breaks=seq(0.0, 0.8, 0.2))


# for xc data

df = read.csv('./compare_xc_acc_rf.csv')
df = read.csv('./compare_xc_acc_cart.csv')
df = read.csv('./compare_xc_acc_svm.csv')
df = read.csv('./compare_xc_acc_ipsm.csv')

df$补样数量 = factor(df$补样数量)
colors<-c('#CCCCCC','#5CACEE','#FF6A6A')
ggplot(data=df)+
  geom_boxplot(mapping = aes(x=补样数量, y=RMSE, fill=补样方法))+
  scale_fill_manual(values = colors)+
  labs(x="补样数量", y="RMSE", title = "")+
  theme_bw()+
  theme(plot.title=element_text(hjust=0.5),
        axis.title.x = element_text(size=14, face = 'bold'),
        axis.title.y = element_text(size=14, face = 'bold'),
        legend.position = 'none',
        legend.text = element_text(size = 13, face = 'bold'),
        legend.title = element_blank()
  )+
  coord_cartesian(ylim=c(8,10))+
  scale_y_continuous(breaks=seq(8.0, 10.0, 0.5))
