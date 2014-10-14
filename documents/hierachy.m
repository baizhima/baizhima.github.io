%% Read in globally used data
load('finalinput.mat') % A,S
rankB = S(:,1:2);
rankO = S(:,3:4);

tic
%% Parameters
expOverEff = 1/2;  % From 1/9 to 9
expPara = [3 9 4 9 7 3 8 1/7 1 7]; % [yrsOverElse(4) ogOverElse(3) opctOverElse(2) bgOverElse(1)] 
effPara = [1/4 1/9 1/5 1/9 1/4 1/5 1/7 2 1 1/3]; % [yrsOverElse(4) ogOverElse(3) opctOverElse(2) bgOverElse(1)] 
n = 5; % number of elements
RI = [0 0 0.58 0.9 1.12 1.24 1.32 1.41 1.45];
%% Level 1 Experience vs. Efficiency
%        Exp Eff
%    Exp  1
%    Eff      1
L1 = eye(2);

L1(1,2) = expOverEff;
L1(2,1) = 1/expOverEff;
L1weight = [1-1/(expOverEff+1) 1/(expOverEff+1)]; % basic formula deriviation
%% Level 2 over exp
%         yrs  og  opct  bg  bpct
%    yrs   1
%    og        1
%    opct           1
%    bg                  1
%    bpct                     1
L2exp = eye(5);

expYrsOverElse = expPara(1,1:4);
expOgOverElse = expPara(1,5:7);
expOpctOverElse = expPara(1,8:9);
expBgOverElse = expPara(1,10);
L2exp(1,2:5) = expYrsOverElse; L2exp(2:5,1) = (1./L2exp(1,2:5))';
L2exp(2,3:5) = expOgOverElse; L2exp(3:5,2) = (1./L2exp(2,3:5))';
L2exp(3,4:5) = expOpctOverElse; L2exp(4:5,3) = (1./L2exp(3,4:5))';
L2exp(4,5) = expBgOverElse; L2exp(5,4) = 1/L2exp(4,5);

%% Level 2 over eff
%         yrs  og  opct  bg  bpct
%    yrs   1
%    og        1
%    opct           1
%    bg                  1
%    bpct                     1
L2eff = eye(5);

effYrsOverElse = effPara(1,1:4);
effOgOverElse = effPara(1,5:7);
effOpctOverElse = effPara(1,8:9);
effBgOverElse = effPara(1,10);
L2eff(1,2:5) = effYrsOverElse; L2eff(2:5,1) = (1./L2eff(1,2:5))';
L2eff(2,3:5) = effOgOverElse; L2eff(3:5,2) = (1./L2eff(2,3:5))';
L2eff(3,4:5) = effOpctOverElse; L2eff(4:5,3) = (1./L2eff(3,4:5))';
L2eff(4,5) = effBgOverElse; L2eff(5,4) = 1/L2eff(4,5);

%% Compute Eigenvalues and Eigenvectors of L2exp and L2eff
[Vexp,Dexp] = eig(L2exp);
expCI=(Dexp(1,1)-n)/(n-1);
expCR = expCI / RI(n);
disp('expCR=');
disp(expCR);
[Veff,Deff] = eig(L2eff);
effCI=(Deff(1,1)-n)/(n-1);
effCR = effCI / RI(n);
disp('effCR=');
disp(effCR);


%% Compute Priority vector of L2exp and L2eff
wexp = zeros(3,1);
weff = zeros(3,1);
% get wexp
expsum = sum(L2exp);
L2exp2 = [];
for i=1:n
    L2exp2 = [L2exp2 L2exp(:,i)/expsum(i)];
end
wexp = sum(L2exp2,2)./n; % Here we go!
% get weff
effsum = sum(L2eff);
L2eff2 = [];
for i=1:n
    L2eff2 = [L2eff2 L2eff(:,i)/effsum(i)];
end
weff = sum(L2eff2,2)./n; % Here we go!
L2weight = [wexp'; weff'];

weight = L1weight*L2weight;
%% Read in 377*6 matrix data, columns named as "id,years,og,opct,bg,bpct"
% return a 377*2 matrix, columns named as "id,score"
A = xlsread('football_new.xlsx'); 

rowlength = size(A,1);

idScore = zeros(size(A,1),2);
idScore(:,1) = A(:,1);
for i=1:size(A,1)
    idScore(i,2) = sum(weight.*A(i,2:2+n-1));
end

   

%% Sort
IdScore = idScore;
number = size(A,1);
IdScore_ranking = sortrows(IdScore,2);
IdScore_ranking = [IdScore_ranking ones(number,1)];
IdScore_ranking(:,3) = number:-1:1;
IdScore_ranking = sortrows(IdScore_ranking,3);
IdScore_ranking(:,3) = 1:number;
IdRank1 = IdScore_ranking(:,1);
IdRank2 = IdScore_ranking(:,3);
IdRank = [IdRank1 IdRank2];

%% Comparing our model with Bleacher Report and Olando Sentinel ranking
idRank = IdRank;
idRank = idRank(1:50,:);
count1_5B = 0;
count1_5O = 0;
for i = 1:5;
    a = idRank(i,1);
    b = min(abs(rankB(1:5,1)-a));
    c = min(abs(rankO(1:5,1)-a));
    if b == 0;
        count1_5B = count1_5B + 1;
    else
        count1_5B = count1_5B;
    end
    if c == 0;
        count1_5O = count1_5O + 1;
    else
        count1_5O = count1_5O;
    end
    clear a
    clear b
    clear c
end
count1_10B = 0;
count1_10O = 0;
for i = 1:10;
    a = idRank(i,1);
    b = min(abs(rankB(1:10,1)-a));
    c = min(abs(rankO(1:10,1)-a));
    if b == 0;
        count1_10B = count1_10B + 1;
    else
        count1_10B = count1_10B;
    end
    if c == 0;
        count1_10O = count1_10O + 1;
    else
        count1_10O = count1_10O;
    end
    clear a
    clear b
    clear c
end
count1_20B = 0;
count1_20O = 0;
for i = 1:20;
    a = idRank(i,1);
    b = min(abs(rankB(1:20,1)-a));
    c = min(abs(rankO(1:20,1)-a));
    if b == 0;
        count1_20B = count1_20B + 1;
    else
        count1_20B = count1_20B;
    end
    if c == 0;
        count1_20O = count1_20O + 1;
    else
        count1_20O = count1_20O;
    end
    clear a
    clear b
    clear c
end
count1_30B = 0;
count1_30O = 0;
for i = 1:30;
    a = idRank(i,1);
    b = min(abs(rankB(1:30,1)-a));
    c = min(abs(rankO(1:30,1)-a));
    if b == 0;
        count1_30B = count1_30B + 1;
    else
        count1_30B = count1_30B;
    end
    if c == 0;
        count1_30O = count1_30O + 1;
    else
        count1_30O = count1_30O;
    end
    clear a
    clear b
    clear c
end
count1_50B = 0;
count1_50O = 0;
for i = 1:50;
    a = idRank(i,1);
    b = min(abs(rankB(1:50,1)-a));
    c = min(abs(rankO(1:50,1)-a));
    if b == 0;
        count1_50B = count1_50B + 1;
    else
        count1_50B = count1_50B;
    end
    if c == 0;
        count1_50O = count1_50O + 1;
    else
        count1_50O = count1_50O;
    end
    clear a
    clear b
    clear c
end
fitB = count1_5B/5+count1_10B/10+count1_20B/20+count1_30B/30+count1_50B/50
fitO = count1_5O/5+count1_10O/10+count1_20O/20+count1_30O/30+count1_50O/50
toc
