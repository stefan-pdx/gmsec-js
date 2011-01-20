FdsDemo.orbitDetermination = SC.Page.design
    mainView: SC.SplitView.design
        layout:
            left: 0
            right: 0
            top: 0
            bottom: 0
        layoutDirection: SC.LAYOUT_HORIZONTAL
        autoresizeBehavior: SC.RESIZE_TOP_LEFT,
        defaultThickness: 0.8
        topLeftView: SC.ScrollView.design
            layout:
                top: 0
                bottom: 0
                left: 0
                right: 0
            hasHorizontalScroller: NO
            backgroundColor: 'white'
            rowHeight: 21
            topLeftMinThickness: 150
            topLeftMaxThickness: 250
            contentView: SC.ListView.design
                contentBinding: 'FdsDemo.residualsArrayController.arrangedObjects'
                selectionBinding: 'FdsDemo.residualsArrayController.selection'
                contentValueKey: 'epoch'
        dividerView: SC.SplitDividerView.design
            layout: {}
        bottomRightView: SC.View.design
            layout:
                top: 0
                bottom: 0
                left: 0
                right: 0
            childViews: 'preUpdateResiduals postUpdateResiduals infoView'.w()
            preUpdateResiduals: Flot.GraphView.design
                layout:
                    left: 0
                    right: 0
                    top: 0
                    bottom: 0.66
                dataBinding: SC.Binding.from 'FdsDemo.maneuverController.iterations'
            postUpdateResiduals: Flot.GraphView.design
                layout:
                    left: 0
                    right: 0
                    top: 0.33
                    bottom:  0.33 
                dataBinding: SC.Binding.from 'FdsDemo.maneuverController.iterations'
            infoView: SC.View.design
                layout:
                    left: 5
                    right: 5
                    top: 0.70
                    bottom: 5
                childViews: 'tableView textView'.w()
                tableView: SC.TableView.design
                    layout:
                        left: 0
                        right: 0.51
                        top: 0
                        bottom: 0
                    columns: [
                        SC.TableColumn.create
                            key: 'statProperty'
                            label: 'Stat Property'
                            width: 100
                        SC.TableColumn.create
                            key: 'statValue'
                            label: 'Stat Value'
                            width: 200]
                textView: SC.ScrollView.design
                    layout:
                        left: 0.51
                        right: 0
                        top: 0
                        bottom: 0
                    backgroundColor: 'white'
                    contentView: SC.LabelView.design
                        content: 'Hello world'
