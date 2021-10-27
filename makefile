all: parallel_conway serial_conway

parallel_conway: parallel_conway.cpp
	mpic++ parallel_conway.cpp -o parallel_conway

serial_conway: serial_conway.cpp
	g++ serial_conway.cpp -o serial_conway

clean:
	rm -f parallel_conway
	rm -f serial_conway