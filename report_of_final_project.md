# Github repo link
https://github.com/sssh311318/ESL_final_project

----------------------------------

# Introduction

我的演算法是binary search ，input data 總共是40個數字，

key的產生方式我是隨機產生一個0~39的index，到我存放資料的array裡面挑出key。

在1core1PE的版本因為方便比較所以我將PE修改成一次處理40筆數字。

在1core2PE和2core2PE的版本，每個PE都是只處理20個數字。

在PE design的部分，我是先用我期中用的演算法 Cocktail Sort。
先做好排序之後再根據binary search 找出我要的key。

----------------------

# Result of simulation and synthesis

    This is a record of PE of area which process 20 numbers

![area_of_pe](https://github.com/sssh311318/ESL_final_project/blob/main/result_of_final_project/PE_area.png?raw=true)

    These are the different result between number of PE and core.


---
    1core1PE
![1core1PE](https://github.com/sssh311318/ESL_final_project/blob/main/result_of_final_project/1core1PE.png?raw=true)
    
    1core2PE
![1core2PE](https://github.com/sssh311318/ESL_final_project/blob/main/result_of_final_project/1core2PE.png?raw=true)

    2core2PE
![2core2PE-1](https://github.com/sssh311318/ESL_final_project/blob/main/result_of_final_project/2core2PE-1.png?raw=true)
![2core2PE-2](https://github.com/sssh311318/ESL_final_project/blob/main/result_of_final_project/2core2PE-2.png?raw=true)

# Summary

從上面的結果可以看出在相同PE數目的情況下，多了1個core simulated time 大幅度的減少，在報告當天經過老師的講解才知道我做的有問題，後來才知道問題是我在makefile裡面忘記把bootstrap.S 一起加進去compile所以我根本沒有用到另一個core，我會發現是因為我嘗試print出hart_id 但是id都是顯示0，然後去比對lab的example才發現這個bug的。
