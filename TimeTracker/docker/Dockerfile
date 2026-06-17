FROM ubuntu:24.04 AS build

ENV DEBIAN_FRONTEND=noninteractive

RUN --mount=type=cache,target=/var/cache/apt \
    --mount=type=cache,target=/var/lib/apt/lists \
    apt-get update && apt-get install -y --no-install-recommends \
    build-essential \
    cmake \
    ninja-build \
    ca-certificates \
    pkg-config \
    ccache \
    catch2 \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app

COPY . .

RUN cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Release \
  -DCMAKE_CXX_COMPILER_LAUNCHER=ccache \
  && cmake --build build --target TimeTracker session_tests task_tests storage_tests timetracker_tests

FROM build AS tests

WORKDIR /app
CMD ["ctest", "--test-dir", "build", "--output-on-failure"]

FROM ubuntu:24.04 AS runtime

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get install -y --no-install-recommends \
  ca-certificates \
  libstdc++6 \
  && rm -rf /var/lib/apt/lists/*

WORKDIR /app

COPY --from=build /app/build/bin/TimeTracker /usr/local/bin/TimeTracker
COPY data.json /app/data.json

ENTRYPOINT ["/usr/local/bin/TimeTracker"]
CMD []
