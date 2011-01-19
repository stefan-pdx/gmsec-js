FdsDemo.maneuverPlanning = SC.Page.design
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
                contentBinding: 'FdsDemo.maneuversArrayController.arrangedObjects'
                selectionBinding: 'FdsDemo.maneuversArrayController.selection'
                contentValueKey: 'epoch'
        dividerView: SC.SplitDividerView.design
            layout: {}
        bottomRightView: SC.View.design
            layout:
                top: 0
                bottom: 0
                left: 0
                right: 0
            childViews: 'plotView infoView'.w()
            plotView: Flot.GraphView.design
                layout:
                    left: 0
                    right: 0
                    top: 0
                    bottom:  0.60 
                dataBinding: SC.Binding.from 'FdsDemo.maneuverController.iterations'
            infoView: SC.View.design
                layout:
                    left: 5
                    right: 5
                    top: 5
                    bottom: 5
                childViews: 'tableView'.w()
                tableView: SC.TableView.design
                    layout:
                        left: 0
                        right: 0.50
                        top: 0.40
                        bottom: 0
                    columns: [
                        SC.TableColumn.create
                            key: 'iterationID'
                            label: 'Iteration'
                            width: 50
                        SC.TableColumn.create
                            key: 'timeToViolation'
                            label: 'Time to Violation'
                            width: 50
                        ]
