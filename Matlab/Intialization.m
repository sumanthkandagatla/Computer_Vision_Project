%Read Background Image
Background=imread('C:\Users\sumanth\Desktop\haha\Back.jpg');
%Read Current Frame
CurrentFrame=imread('C:\Users\sumanth\Desktop\haha\LK.jpg');
%Display Background and Foreground
subplot(2,2,1);imshow(Background);title('BackGround');
%Convert RGB 2 HSV Color conversion
[Background_hsv]=round(rgb2hsv(Background));
[CurrentFrame_hsv]=round(rgb2hsv(CurrentFrame));
%CurrentFrame=imread('C:\Users\sumanth\Desktop\Right.jpg');
Out = bitxor(Background_hsv,CurrentFrame_hsv);
%Convert RGB 2 GRAY
Out=rgb2gray(Out);

%Read Rows and Columns of the Image
[rows, columns]=size(Out);
%Convert to Binary Image
for i=1:rows
for j=1:columns

if Out(i,j) >0

binaryImage(i,j)=1;

else

binaryImage(i,j)=0;

end

end
end
%Apply Median filter to remove Noise
FilteredImage=medfilt2(binaryImage,[5 5]);

%Boundary Label the Filtered Image
[L1, num]=bwlabel(FilteredImage);

STATS=regionprops(L1,'all');
c=[];
removed=0;

%Remove the noisy regions 
for i=1:num
dd=STATS(i).Area;

if (dd < 500)

L1(L1==i)=0;
removed = removed + 1;
num2=num-1;

else

end

end

[L2, num]=bwlabel(L1);

% Trace region boundaries in a binary image.

[B,L,N,A] = bwboundaries(L2);
%Display results

subplot(2,2,2),  imshow(L2);title('BackGround Detected');
imwrite(L2,'C:\Users\sumanth\Desktop\haha\Lg.jpeg');
subplot(2,2,3),  imshow(L2);title('Blob');

hold on;
for k=1:length(B)

if(~sum(A(k,:)))
boundary = B{k};
plot(boundary(:,2), boundary(:,1), 'r','LineWidth',2);

for l=find(A(:,k))'
boundary = B{l};
plot(boundary(:,2), boundary(:,1), 'g','LineWidth',2);
end

end,

end
