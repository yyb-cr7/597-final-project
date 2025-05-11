
all: build run-dsdv run-aodv run-zrp


build:
	./waf build


run-dsdv:
	./waf --run scratch/dsdv-final


run-aodv:
	./waf --run scratch/aodv-final


run-zrp:
	./waf --run scratch/zrp-final
