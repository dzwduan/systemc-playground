# non-blocking transport demo

nb_transport 常用于 perf model，最基础的结构如下

Req

+ initiator [register_nb_transport_bw] ---> target.nb_tranport_fw ---> target [register_nb_transport_fw]

+ register_nb_transport_bw <--- initiator.nb_transport_bw <--- target

Resp

+ initiator [nb_transport_bw]   <----> target [nb_transport_fw]
+ initiator <--- initiator.nb_transport_bw  <--- target
+ initiator ---> target.nb_transport_fw     ---> target 




## refer 

https://blog.csdn.net/zgcjaxj/article/details/126533805