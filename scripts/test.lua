


for i=0,12 do
	mBox(i*64, 400)
end



pos = createVec2(100,100);
--p = player(pos);

function p()
	player(getMousePos());
end

function createBoxes(x, y, n)
	for i=0,n do
		mBox(i*x, y)
	end
end

function createBoxes2(pos, n)
	for i=0,n do
		mBox(i*pos.x, pos.y)
	end
end

