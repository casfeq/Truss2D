import numpy as np
import pathlib
import matplotlib.pyplot as plt
import sys

"""    PLOT RESULTS
  ----------------------------------------------------------------"""

# Get filenames
# solvedPairs=[]
# for eachArg in sys.argv:
# 	solvedPairs.append(eachArg)
# solvedPairs.pop(0)

# Get parent directory
parentDirectory=pathlib.Path(__file__).resolve().parents[1]

# Define colors
colors=[]
colors.append("#fd411e")
colors.append("#f075e6")
colors.append("#0d75f8")
colors.append("#02c14d")

# Create and define figure's size and margins
fig=plt.figure()
fig.clf()
fig.add_subplot(1,1,1)

# Define figure's name
plotName="plot/trussDeformed.png"

# Plot initial position
connectivity=np.loadtxt(fname=str(parentDirectory)+"/input/connectivity.txt")
coordinates=np.loadtxt(fname=str(parentDirectory)+"/input/coordinates.txt")
for element in range(0,len(connectivity)):
	xData=[]
	yData=[]
	node1=int(connectivity[element][0])-1
	node2=int(connectivity[element][1])-1
	xData.append(coordinates[node1][0])
	xData.append(coordinates[node2][0])
	yData.append(coordinates[node1][1])
	yData.append(coordinates[node2][1])
	if element==0:
		plt.plot(xData,yData,'-o',color=colors[3],ms=5,mec='k',mew=0.5,label='Inital Position')
	else:
		plt.plot(xData,yData,'-o',color=colors[3],ms=5,mec='k',mew=0.5)

# Plot deformed position
displacement=np.loadtxt(fname=str(parentDirectory)+"/export/trussSolution.txt")
for element in range(0,len(connectivity)):
	xData=[]
	yData=[]
	node1=int(connectivity[element][0])-1
	node2=int(connectivity[element][1])-1
	disp1x=displacement[2*node1]
	disp1y=displacement[2*node1+1]
	disp2x=displacement[2*node2]
	disp2y=displacement[2*node2+1]
	xData.append(coordinates[node1][0]+disp1x)
	xData.append(coordinates[node2][0]+disp2x)
	yData.append(coordinates[node1][1]+disp1y)
	yData.append(coordinates[node2][1]+disp2y)
	if element==0:
		plt.plot(xData,yData,'--o',color=colors[0],mec='k',mew=0.5,ms=5,label='Deformed Position')
	else:
		plt.plot(xData,yData,'--o',color=colors[0],mec='k',mew=0.5,ms=5)

# Set axes' labels
plt.xlabel('x (m)')
plt.ylabel('y (m)')
plt.gca().set_aspect('equal', adjustable='box')
plt.grid(which='major',axis='both')

# Add figure's legend
fig.legend(loc='upper center',ncol=2)

# Save figure
plt.savefig(plotName)

print("Plotted deformed position.")

# Create and define figure's size and margins
fig=plt.figure()
fig.clf()
fig.add_subplot(1,1,1)

# Define figure's name
plotName="plot/trussForces.png"

# Plot initial position
connectivity=np.loadtxt(fname=str(parentDirectory)+"/input/connectivity.txt")
coordinates=np.loadtxt(fname=str(parentDirectory)+"/input/coordinates.txt")
maxPositionX=0
maxPositionY=0
for element in range(0,len(connectivity)):
	xData=[]
	yData=[]
	node1=int(connectivity[element][0])-1
	node2=int(connectivity[element][1])-1
	xData.append(coordinates[node1][0])
	xData.append(coordinates[node2][0])
	yData.append(coordinates[node1][1])
	yData.append(coordinates[node2][1])
	maxPositionX=max(abs(maxPositionX),abs(coordinates[node1][0]))
	maxPositionY=max(abs(maxPositionY),abs(coordinates[node1][1]))
	maxPositionX=max(abs(maxPositionX),abs(coordinates[node2][0]))
	maxPositionY=max(abs(maxPositionY),abs(coordinates[node2][1]))
	if element==0:
		plt.plot(xData,yData,'-',color=colors[0],label='Reaction forces')
		plt.plot(xData,yData,'-',color=colors[2],label='Applied forces')
	plt.plot(xData,yData,'-o',color=colors[3],ms=5,mec='k',mew=0.5)
vecsize=min(maxPositionX,maxPositionY)/2

# Plot applied forces
applied=np.loadtxt(fname=str(parentDirectory)+"/input/neumannBC.txt")
maxForce=0
for node in range(0,len(applied)):
	xPosition=coordinates[int(applied[node][0])-1][0]
	yPosition=coordinates[int(applied[node][0])-1][1]
	force=applied[node][2]
	maxForce=max(maxForce,abs(force))
	if force!=0 and applied[node][1]==1:
		plt.gca().annotate('',xy=(xPosition,yPosition),xytext=(xPosition+vecsize*force/ \
			abs(force),yPosition),arrowprops=dict(arrowstyle='<-',color=colors[2],lw=2))
		plt.gca().annotate(str(format(abs(reactionX),'.1e'))+" N",xy=(xPosition,yPosition), \
			xytext=(xPosition+vecsize*force/abs(force),yPosition))
	if force!=0 and applied[node][1]==2:
		plt.gca().annotate('',xy=(xPosition,yPosition),xytext=(xPosition,yPosition+vecsize* \
			force/abs(force)),arrowprops=dict(arrowstyle='<-',color=colors[2],lw=2))
		plt.gca().annotate(str(format(abs(force),'.1e'))+" N",xy=(xPosition,yPosition), \
			xytext=(xPosition,yPosition+vecsize*force/abs(force)))

# Plot reaction forces
reactions=np.loadtxt(fname=str(parentDirectory)+"/export/trussReactions.txt")
for node in range(0,len(coordinates)):
	if abs(reactions[2*node])<1e-6*maxForce:
		reactions[2*node]=0
	if abs(reactions[2*node+1])<1e-6*maxForce:
		reactions[2*node+1]=0
for node in range(0,len(coordinates)):
	xPosition=coordinates[node][0]
	yPosition=coordinates[node][1]
	reactionX=reactions[2*node]
	reactionY=reactions[2*node+1]
	if reactionX!=0:
		plt.gca().annotate('',xy=(xPosition,yPosition),xytext=(xPosition+vecsize*reactionX/ \
			abs(reactionX),yPosition),arrowprops=dict(arrowstyle='<-',color=colors[0],lw=2))
		plt.gca().annotate(str(format(abs(reactionX),'.1e'))+" N",xy=(xPosition,yPosition), \
			xytext=(xPosition+vecsize*reactionX/abs(reactionX),yPosition))
	if reactionY!=0:
		plt.gca().annotate('',xy=(xPosition,yPosition),xytext=(xPosition,yPosition+vecsize* \
			reactionY/abs(reactionY)),arrowprops=dict(arrowstyle='<-',color=colors[0],lw=2))
		plt.gca().annotate(str(format(abs(reactionX),'.1e'))+" N",xy=(xPosition,yPosition), \
			xytext=(xPosition,yPosition+vecsize*reactionY/abs(reactionY)))

# Set axes' labels
plt.xlabel('x (m)')
plt.ylabel('y (m)')
plt.gca().set_aspect('equal', adjustable='box')
plt.grid(which='major',axis='both')

# Add figure's legend
fig.legend(loc='upper center',ncol=2)

# Save figure
plt.savefig(plotName)

print("Plotted reaction forces.")