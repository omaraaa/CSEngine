


for i=0,12 do
	mBox(i*64, 400)
end



pos = createVec2(100,100);
pl = player(pos);
follow(pl);
plmc = getMC(pl);
p2 = plmc.maxVel
p2.y = 100000000
plmc.maxVel = p2
p2 = plmc.acc
p2.y = p2.y * 1
plmc.acc = p2
plmc.acc.y = plmc.acc.y * 4

function p()
	return player(getMousePos());
end

function createBoxes(x, y, n)
	for i=0,n-1 do
		mBox(i*64+x, y)
	end
end

function createBoxes2(pos, n)
	for i=0,n do
		mBox(i*pos.x, pos.y)
	end
end

function update(id)
	
end

