FROM ubuntu:20.04

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get install -y \
    build-essential \
    pkg-config \
    libfuse-dev \
    fuse \
    iproute2 \
    procps \
    nano \
    vim \
    curl \
    sudo \
    && rm -rf /var/lib/apt/lists/*

RUN mkdir -p /it24_host /antink_mount /antink-logs

COPY antink.c /antink.c

RUN gcc -Wall -o /antink /antink.c `pkg-config fuse --cflags --libs`

RUN echo '#!/bin/bash\n\
mkdir -p /it24_host /antink_mount /antink-logs\n\
touch /antink-logs/it24.log\n\
if [ "$1" = "server" ]; then\n\
  /antink /antink_mount -f -d\n\
elif [ "$1" = "logger" ]; then\n\
  tail -f /antink-logs/it24.log\n\
else\n\
  exec "$@"\n\
fi' > /start.sh && chmod +x /start.sh

ENTRYPOINT ["/start.sh"]
CMD ["server"]
