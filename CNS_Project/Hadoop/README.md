# **Hadoop Map-Reduce**

Project for **Computer Networks**

## **Brief Description**

Apache Hadoop is a collection of open-source software utilities that facilitate using a network of many computers to solve problems involving massive amounts of data and computation. It provides a software framework for distributed storage and processing of big data using the MapReduce programming model. The main objective of this project is to implement map-reduce model and realize word count task.

## **Project Structure**

1. Implement word count model working on a single process using Java
2. Create *Master/Slave* architecture and deploy it on several docker containers
3. Write job distribution controller using shell
4. Evaluate the performance of distributed model base on time cost

For more information, please view the [report](Report.pdf)

## **Usage**

### **Docker image**

```shell
sudo docker pull yiping999/ub-jdk8:1.1
```

### **Disable SSH check**

it seems this command doesn't work as expected

```shell
ssh -o "StrictHostKeyChecking=no" worker1
```

### **Useful command**

- compile BASIC.jar SLAVE.jar MASTER.jar

```shell
sh deploy.sh compile
```

or compile each file seperately

```shell
sh deploy.sh compile basic
sh deploy.sh compile slave
sh deploy.sh compile master
```

- run BASIC.jar on a given directory

```shell
sh deploy.sh basic dirName
```

- run MASTER.jar on a given directory

```shell
sh deploy.sh master dirName
```

- delete intermediate files inside containers

```shell
sh clean.sh container
```

- delete intermediate and final files on the host

```shell
sh clean.sh host
```

- stop all containers

```shell
sh clean.sh stop
```

- delete all host generable files

```shell
sh clean.sh all
```
