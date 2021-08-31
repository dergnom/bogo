def aifunc(Player,otherPlayer,Ball,Container):
	xPos=Player[0]
	xBall=Ball[0]
	r=0
	l=0
	j=0
	
	#print xPos
	if xPos > 400:
		
		if xBall < 400:
			if xPos < 590:
				r=1 
			elif xPos > 610:
				l=1
		elif xBall > 400:
			if xPos > xBall+10+5:
				l=1
			elif xPos < xBall+10-5:
				r=1
	elif xPos < 400:
		
		if xBall > 400:
			if xPos > 210:
				l=1
			elif xPos < 190:
				r=1
		elif xBall < 400:
			if xPos > xBall-10+2:
				l=1
			elif xPos < xBall-10-2:
				r=1
	
	back=(r,l,j,Container)
	return(back)
