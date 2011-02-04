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
        topLeftView: SC.View.design
            layout:
                top: 0
                bottom: 0
                left: 0
                right: 0
            backgroundColor: 'white'
            childViews: 'listView scaleFactorView smaView'.w()
            listView: SC.ScrollView.design
                layout:
                    top: 0
                    bottom: 0.5
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
                    contentValueKey: 'title'
            scaleFactorView: Flot.GraphView.design
                layout:
                    top: 0.5
                    bottom: 0.25
                    left: 0
                    right: 0
            smaView: Flot.GraphView.design
                layout:
                    top: 0.25
                    bottom: 0
                    left: 0
                    right: 0
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
                    bottom:  0.33 
                dataBinding: SC.Binding.from 'FdsDemo.maneuverController.iterations'
                options: SC.Object.create
                    yaxis:
                        min: -10
                        max: 10
            infoView: SC.View.design
                layout:
                    left: 5
                    right: 5
                    top: 5
                    bottom: 5
                childViews: 'infoTableView'.w()
                infoTableView: SC.TableView.design
                    layout:
                        left: 0
                        right: 0
                        top: 0.68
                        bottom: 0
                    columns: [
                        SC.TableColumn.create
                            key: 'property'
                            label: 'Property'
                            width: 150
                        SC.TableColumn.create
                            key: 'plannedValue'
                            label: 'Planned Value'
                            width: 150
                        SC.TableColumn.create
                            key: 'reconstructionValue'
                            label: 'Reconstruction Value'
                            width: 150
                        SC.TableColumn.create
                            key: 'calibrationValue'
                            label: 'Calibration Value'
                            width: 150
                        ]
                    contentBinding: SC.Binding.from 'FdsDemo.maneuverController.properties'
