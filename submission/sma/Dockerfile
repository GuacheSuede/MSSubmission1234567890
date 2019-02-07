FROM ubuntu

MAINTAINER Justin Yeap <iamjustinyeap@gmail.com>
  

WORKDIR /app
COPY . /app


RUN apt-get update \
  && apt-get install -y git \
                        g++ \
                        make \
                        wget \ 
                        cmake

RUN cd boost_1_69_0 \
   && ./bootstrap.sh --prefix=/usr/local \
   && ./b2 --with-system \
   && ./b2 install --with-system \
   && cd .. \
   && rm -rf boost_1_69_0


RUN cmake . \
  && make 

EXPOSE 8080

CMD ["./main"]

