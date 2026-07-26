FROM gcc:13
WORKDIR /app
COPY main.cpp .
RUN g++ -std=c++17 -fopenmp -O3 main.cpp -o ants_sim
ENTRYPOINT ["./ants_sim"]