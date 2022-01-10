$$sample11pp	START
	LAD	gr0, 0
	CALL	L0001
	CALL	FLUSH
	SVC	0
$n%kazuyomikomi	DC	0
$kazuyomikomi
	POP	gr2
	POP	gr1
	ST	gr1, $n%kazuyomikomi
	PUSH	0, gr2
$sum% 	DC	0
$wakakidasi
	POP	gr2
	POP	gr1
	ST	gr1, $n%wakakidasi
	PUSH	0, gr2
$data% 	DC	0
$n%goukei	DC	0
$s%goukei	DC	0
$data%goukei	DC	0
$goukei
	POP	gr2
	POP	gr1
	ST	gr1, $n%goukei
	POP	gr1
	ST	gr1, $s%goukei
	PUSH	0, gr2
	RET
$n% 	DC	0
