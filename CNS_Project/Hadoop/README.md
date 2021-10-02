# **Map-Reduce**

## **Docker image**

```shell
sudo docker pull yiping999/ub-jdk8:1.1
```

## **Disable SSH check**

it seems this command doesn't work as expected

```shell
ssh -o "StrictHostKeyChecking=no" worker1
```

## **Usage**

* compile BASIC.jar SLAVE.jar MASTER.jar

```shell
sh deploy.sh compile
```

or  

```shell
sh deploy.sh compile basic
sh deploy.sh compile slave
sh deploy.sh compile master
```

* run BASIC.jar on a given directory

```shell
sh deploy.sh basic dirName
```

* run MASTER.jar on a given directory

```shell
sh deploy.sh master dirName
```

* delete intermediate files inside containers

```shell
sh clean.sh container
```

* delete intermediate and final files on the host

```shell
sh clean.sh host
```

* stop all containers

```shell
sh clean.sh stop
```

* delete all host generable files

```shell
sh clean.sh all
```
