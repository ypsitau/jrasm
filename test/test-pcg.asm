	.org	0x1000
	
	pcgpage.circles.store

	pcg.circle1x1.put
	pcg.dblcircle1x1.put
	pcg.circle2x2.put
	pcg.dblcircle2x2.put
	pcg.chkcircle2x2.put
	pcg.blank2x1.put
	;pcg.circle1x1.putattr 7,0
	;pcg.circle1x1.erase
	
	.pcgpage circles,user,60
	
	.pcg	circle1x1,1,1
	.db	b"..####.."
	.db	b".#....#."
	.db	b"#......#"
	.db	b"#......#"
	.db	b"#......#"
	.db	b"#......#"
	.db	b".#....#."
	.db	b"..####.."
	.end

	.pcg	dblcircle1x1,1,1
	.db	b"..####.."
	.db	b".#....#."
	.db	b"#..##..#"
	.db	b"#.#..#.#"
	.db	b"#.#..#.#"
	.db	b"#..##..#"
	.db	b".#....#."
	.db	b"..####.."
	.end
	
	.pcg	circle2x2,2,2
	.db	b".....######....."
	.db	b"...##......##..."
	.db	b"..#..........#.."
	.db	b".#............#."
	.db	b".#............#."
	.db	b"#..............#"
	.db	b"#..............#"
	.db	b"#..............#"
	.db	b"#..............#"
	.db	b"#..............#"
	.db	b"#..............#"
	.db	b".#............#."
	.db	b".#............#."
	.db	b"..#..........#.."
	.db	b"...##......##..."
	.db	b".....######....."
	.end
	
	.pcg	dblcircle2x2,2,2
	.db	b".....######....."
	.db	b"...##......##..."
	.db	b"..#..........#.."
	.db	b".#............#."
	.db	b".#....####....#."
	.db	b"#....#....#....#"
	.db	b"#...#......#...#"
	.db	b"#...#......#...#"
	.db	b"#...#......#...#"
	.db	b"#...#......#...#"
	.db	b"#....#....#....#"
	.db	b".#....####....#."
	.db	b".#............#."
	.db	b"..#..........#.."
	.db	b"...##......##..."
	.db	b".....######....."
	.end
	
	.pcg	blank2x1,2,1
	.db	b"................"
	.db	b"................"
	.db	b"................"
	.db	b"................"
	.db	b"................"
	.db	b"................"
	.db	b"................"
	.db	b"................"
	.end
	
	.pcg	chkcircle2x2,2,2
	.db	b".....######....."
	.db	b"...##...#####..."
	.db	b"..#.....######.."
	.db	b".#......#######."
	.db	b".#......#######."
	.db	b"#.......########"
	.db	b"#.......########"
	.db	b"#.......########"
	.db	b"########.......#"
	.db	b"########.......#"
	.db	b"########.......#"
	.db	b".#######......#."
	.db	b".#######......#."
	.db	b"..######.....#.."
	.db	b"...#####...##..."
	.db	b".....######....."
	.end
	
	.end
