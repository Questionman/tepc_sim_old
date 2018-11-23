with open('run1.mac', 'r+') as f: 
  lines=[]
  new_line ='/gps/hist/point'
  for line in f:
    if not(line.startswith('#')):
      lines = lines + [new_line]
    else :
      lines = lines + [line]
  f.seek(0)
  f.writelines(lines)
  f.truncate()



