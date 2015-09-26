#!/bin/sh

port=1234

(
while [ true ]
do
	# Make list of IP addresses with service open
	sudo nmap -sS --open -p$port 10.250.100.100-254 > map || exit 1
	grep Interesting map | sed -e 's/://' | awk '{print $4}' > IP || exit 1

	# Show output of each service
	echo
	date
	echo "Scan found "`wc -l IP`" hosts:"
	for host in `cat IP`
	do
		name=`grep "$host " names`
		if [ "$name" ]
		then
			echo -n "$name: "
		else
			echo -n "$host: "
		fi
		response=`(sleep 1; echo "xeyes &") | nc -w 2 "$host" "$port"`
		echo "$response"
	done
	sleep 15 || exit 1
done
) | tee log
