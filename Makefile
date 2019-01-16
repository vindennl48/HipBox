run:
	./jack_server/start_jack.sh
	guitarix -J --name=james -p 4000 &
	guitarix -J --name=jesse -p 4001 &
	guitarix -J --name=mitch -p 4002 &
	rvmsudo rails s -d -p 80 -b 0.0.0.0
	python3 jack_server/main.py

rails:
	rvmsudo rails s -d -p 80 -b 0.0.0.0

run_headless:
	./jack_server/start_jack.sh
	guitarix -N -J --name=james -p 4000 &
	guitarix -N -J --name=jesse -p 4001 &
	guitarix -N -J --name=mitch -p 4002 &
	rvmsudo rails s -d -p 80 -b 0.0.0.0
	python3 jack_server/main.py

run_start_jack:
	./jack_server/start_jack.sh

run_guitarix_headless:
	guitarix -N -J --name=james -p 4000 &
	guitarix -N -J --name=jesse -p 4001 &
	guitarix -N -J --name=mitch -p 4002 &

run_rails:
	rvmsudo rails s -d -p 80 -b 0.0.0.0

run_jack_server:
	python3 jack_server/main.py

