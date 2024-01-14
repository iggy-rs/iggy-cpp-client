# Iggy C++ Client

This project aims to create a full C++ clients for [Iggy.rs](https://iggy.rs). It is as much a learning project as a fully functional one.

## HOWTO: test GitHub actions locally

This VSCode container includes act and Docker-in-Docker, and you can use it to do a smoke test of the CI actions before you push to GitHub:

```
act -P ubuntu-20.04=catthehacker/ubuntu:full-20.04 --matrix=os:ubuntu-20.04 --no-pull
```

Note the Docker image required is very large, and you probably want to pull it once:

```
docker pull catthehacker/ubuntu:full-20.04
```