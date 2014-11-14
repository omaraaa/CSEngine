


for i=0,12 do
	mBox(i*64, 400)
end



pos = createVec2(100,100);
pl = player(pos);
follow(pl);

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
	mc = getMC(id)
	pos2 = mc.acc
	pos2.y = getMousePos().y - mc.pos.y
	pos2.x = getMousePos().x - mc.pos.x
	mc.acc = pos2
end
