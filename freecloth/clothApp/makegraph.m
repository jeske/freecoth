% Matlab function to produce a plot of energy from the stretch/shear/bend
% energy files. Generates EPS, which can then be converted to antialiased
% PNG.
%
% Expects /tmp/{stretch,shear,bend}.txt to contain the energies for a group
% of parameter values. Scales the graph to include the full group, not just
% this dataset.
%
% Also calls makestepgraph.
function result = makegraph( prefix )
    bendscale = 1;
    temp = load( '/tmp/stretch.txt' )';
    maxval = max(max(temp));
    temp = load( '/tmp/shear.txt' );
    maxval = max(maxval,max(max(temp)));
    temp = load( '/tmp/bend.txt' );
    maxval = max(maxval,max(max(temp/bendscale)));

    st = load( [ prefix 'stretch.txt' ] )';
    sh = load( [ prefix 'shear.txt' ] )';
    b = load( [ prefix 'bend.txt' ] )';
    x = 0:0.04:10;
    x=x(1:size(st,1));

    clf;
    % Find a set of ticks that accomodates all data, including maxval.
    plot([0;0.04],[0;maxval],'r+');
    ax = get(gcf,'CurrentAxes');
    ytick = get(ax,'YTick');
    ylim = get(ax,'YLim');
    clf;

    hold on;
    % Plot 'em all.
    lines = plot(x,st,'r-',x,sh,'g-',x,b,'b-');
    % Tweak axes.
    ax = get(gcf,'CurrentAxes');
    set(ax,'Box','on');
    set(ax,'FontSize',14);
    set(ax,'XGrid','on');
    set(ax,'YGrid','on');
    set(ax,'YLim',ylim);
    set(ax,'YTick',ytick);
    yl = get(ax,'YLabel');
    set(yl,'String','Energy');
    set(yl,'FontSize',14);
    xl = get(ax,'XLabel');
    set(xl,'String','Time');
    set(xl,'FontSize',14);
    hold off;
    % Make EPS. Matlab has terrible PNG generation, with no antialiasing.
    print( gcf,'-depsc',[ prefix 'graph.eps' ] );

    set(ax,'FontSize',20);
    set(ax,'YTick',get(ax,'YLim'));
    set(ax,'YGrid','off');
    set(ax,'YMinorTick','on');
    set(ax,'YMinorGrid','on');
    set(yl,'FontSize',36);
    xtick = get(ax,'XTick');
    set(ax,'XTick',get(ax,'XLim'));
    set(ax,'XGrid','off');
    set(ax,'XMinorTick','on');
    set(ax,'XMinorGrid','on');
    set(xl,'FontSize',36);
    p = get(xl,'Position');
    p(2) = p(2) / 3;
    set(xl,'Position',p);
    set(lines(1),'LineWidth',10);
    set(lines(2),'LineWidth',10);
    set(lines(3),'LineWidth',10);

    % Make EPS. Matlab has terrible PNG generation, with no antialiasing.
    print( gcf,'-depsc',[ prefix 'graph_big.eps' ] );

    makestepgraph( prefix );
