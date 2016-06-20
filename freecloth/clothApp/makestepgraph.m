% Matlab function to produce a plot of avg step length. Generates EPS, which
% can then be converted to antialiased PNG.
function result = makestepgraph( prefix )
    maxval = 0.04;
    temp = load( '/tmp/step.txt' );
    maxvalsub = max(max(temp(:,2)));

    s = load( [ prefix 'step.txt' ] );
    x = 0:0.04:10;
    x=x(1:size(s,1));

    clf;
    % Find a set of ticks that accomodates all data, including maxval.
    plot([0;0.04],[0;maxval],'r+');
    ax = get(gcf,'CurrentAxes');
    ytick = get(ax,'YTick');
    ylim = get(ax,'YLim');
    clf;
    % Find a set of ticks that accomodates all data, including maxvalsub.
    plot([0;0.04],[0;maxvalsub],'r+');
    ax = get(gcf,'CurrentAxes');
    yticksub = get(ax,'YTick');
    ylimsub = get(ax,'YLim');
    clf;

    [ax,h1,h2] = plotyy(x,s(:,1),x,s(:,2));
    % Set step attributes
    set(h1,'Color',[1 0 0]);
    set(ax(1),'Box','on');
    set(ax(1),'YColor',[1 0 0]);
    set(ax(1),'FontSize',14);
    set(ax(1),'YLim',ylim);
    set(ax(1),'YTick',ytick);
    yl1 = get(ax(1),'YLabel');
    set(yl1,'String','h_{mean}');
    set(yl1,'FontSize',14);
    % Set substep attributes
    set(h2,'Color',[0 0 1]);
    set(ax(2),'YColor',[0 0 1]);
    set(ax(2),'FontSize',14);
    set(ax(2),'YLim',ylimsub);
    set(ax(2),'YTick',yticksub);
    yl2 = get(ax(2),'YLabel');
    set(yl2,'String','# substeps');
    set(yl2,'FontSize',14);
    xl = get(ax(1),'XLabel');
    set(xl,'String','Time');
    set(xl,'FontSize',14);
    % Make EPS. Matlab has terrible PNG generation, with no antialiasing.
    print( gcf,'-depsc',[ prefix 'stepgraph.eps' ] );

    set(ax,'FontSize',20);
    set(ax,'LineWidth',2);
    ylim = get(ax(1),'YLim');
    set(ax(1),'YTick',ylim(2));
    ylim = get(ax(2),'YLim');
    set(ax(2),'YTick',ylim(2));
    set(yl1,'FontSize',36);
    p = get(yl1,'Position')
    p(1) = p(1) + .3;
    set(yl1,'Position',p);
    set(yl2,'FontSize',36);
    p = get(yl2,'Position')
    p(1) = p(1) - .3;
    set(yl2,'Position',p);
    set(ax,'XTickMode','manual');
    set(ax(1),'XTick',get(ax(1),'XLim'));
    set(ax(2),'XTick',[]);
    set(xl,'FontSize',36);
    p = get(xl,'Position');
    p(2) = p(2) / 3;
    set(xl,'Position',p);
    set(h1,'LineWidth',7);
    set(h2,'LineWidth',7);
    print( gcf,'-depsc',[ prefix 'stepgraph_big.eps' ] );

