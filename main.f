module ex15p6Subs
contains
    
    subroutine angleCalculation(numElement,coord,conec,angle)

        real*4, dimension(1:5,1:2)  :: coord
        integer, dimension(1:7,1:2) :: conec
        integer                     :: numElement,I,J
        real*4                      :: angle,xI,xJ,yI,yJ

        I=conec(numElement,1)
        J=conec(numElement,2)
        xI=coord(I,1)
        yI=coord(I,2)
        xJ=coord(J,1)
        yJ=coord(J,2)
        angle=atan((yJ-yI)/(xJ-xI))

    end subroutine angleCalculation

    subroutine elementStiffnessMatrix(E,A,L,angle,stiffElement)

        real*4                      :: angle
        real*4, dimension(1:4,1:4)  :: stiffElement
        real*4, dimension(1:4,1:4)  :: rotationMatrix
        real*4                      :: pi=3.14159265359
        real*4                      :: E,A,L

        stiffElement=(E*A)/L
        rotationMatrix=rotationMatrix*0.0
        rotationMatrix(1,1)=cos(angle)
        rotationMatrix(1,2)=sin(angle)
        rotationMatrix(2,1)=-sin(angle)
        rotationMatrix(2,2)=cos(angle)
        rotationMatrix(3,3)=cos(angle)
        rotationMatrix(3,4)=sin(angle)
        rotationMatrix(4,3)=-sin(angle)
        rotationMatrix(4,4)=cos(angle)

        stiffElement=matmul(stiffElement,rotationMatrix)
        stiffElement=matmul(transpose(rotationMatrix),stiffElement)

    end subroutine elementStiffnessMatrix

    subroutine stiffnessMatrixOverlap(conec,numElement,stiffElement,stiffGlobal)

        real*4, dimension(1:4,1:4)      :: stiffElement
        real*4, dimension(1:10,1:10)    :: stiffGlobal
        integer                         :: numElement,I,J,l,m
        integer, dimension(1:7,1:2)     :: conec

        I=2*conec(numElement,1)-1
        J=2*conec(numElement,2)-1

        do l=0,1
            do m=0,1
                stiffGlobal(I+l,I+m)=stiffGlobal(I+l,I+m)+stiffElement(l+1,m+1)
                stiffGlobal(I+l,J+m)=stiffGlobal(I+l,I+m)+stiffElement(l+1,m+3)
                stiffGlobal(J+l,I+m)=stiffGlobal(I+l,I+m)+stiffElement(l+3,m+1)
                stiffGlobal(J+l,J+m)=stiffGlobal(I+l,I+m)+stiffElement(l+3,m+3)
            end do
        end do

    end subroutine stiffnessMatrixOverlap

end module ex15p6Subs

program exercicio15p6

    use ex15p6Subs

    logical                         :: file_exists
    real*4, dimension(1:4,1:4)      :: stiffElement
    real*4, dimension(1:4)          :: exportStiffElement
    real*4, dimension(1:5,1:2)      :: coord
    integer, dimension(1:7,1:2)     :: conec
    integer                         :: i,j
    real*4, dimension(1:2)          :: exportCoord
    integer, dimension(1:2)         :: exportConec
    real*4, dimension(1:10,1:10)    :: stiffGlobal
    real*4, dimension(1:10)         :: exportStiffGlobal
    real*4                          :: E=200e6,A=100e-6,L=1.0,angle

    stiffGlobal=0.0

    coord(1,1)=0
    coord(1,2)=0
    coord(2,1)=0.5
    coord(2,2)=0.5*(3**0.5)
    coord(3,1)=1
    coord(3,2)=0
    coord(4,1)=1.5
    coord(4,2)=(3**0.5)*0.5
    coord(5,1)=2
    coord(5,2)=0

    inquire(file="coordenadas.txt",exist=file_exists)
    if(file_exists.eqv..true.) then
        open(unit=1,file="coordenadas.txt",form="formatted",status="replace",action="write")
    else
        open(unit=1,file="coordenadas.txt",form="formatted",status="new",action="write")
    end if
    do i=1,5
        do j=1,2
            exportCoord(j)=coord(i,j)
        end do
        write(1,*) exportCoord(:)
    end do
    close(1)

    conec(1,1)=1
    conec(1,2)=2
    conec(2,1)=1
    conec(2,2)=3
    conec(3,1)=2
    conec(3,2)=3
    conec(4,1)=2
    conec(4,2)=4
    conec(5,1)=3
    conec(5,2)=4
    conec(6,1)=3
    conec(6,2)=5
    conec(7,1)=4
    conec(7,2)=5

    inquire(file="conectividade.txt",exist=file_exists)
    if(file_exists.eqv..true.) then
        open(unit=1,file="conectividade.txt",form="formatted",status="replace",action="write")
    else
        open(unit=1,file="conectividade.txt",form="formatted",status="new",action="write")
    end if
    do i=1,7
        do j=1,2
            exportConec(j)=conec(i,j)
        end do
        write(1,*) exportConec(:)
    end do
    close(1)

    do i=1,7
        call angleCalculation(i,coord,conec,angle)
        call elementStiffnessMatrix(E,A,L,angle,stiffElement)
        call stiffnessMatrixOverlap(conec,i,stiffElement,stiffGlobal)
    end do

    inquire(file="matrizRigidezGlobal.txt",exist=file_exists)
    if(file_exists.eqv..true.) then
        open(unit=1,file="matrizRigidezGlobal.txt",form="formatted",status="replace",action="write")
    else
        open(unit=1,file="matrizRigidezGlobal.txt",form="formatted",status="new",action="write")
    end if
    do i=1,10
        do j=1,10
            exportStiffGlobal(j)=stiffGlobal(i,j)
        end do
        write(1,*) exportStiffGlobal(:)
    end do
    close(1)

end program exercicio15p6