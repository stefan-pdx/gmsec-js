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
         hasHorizontalScroller: NO
         layout:
            top: 0
            bottom: 0
            left: 0
            right: 0
         backgroundColor: 'white'
         contentView: SC.ListView.design
            contentBinding: 'FdsDemo.maneuversArrayController.arrangedObjects'
            selectionBinding: 'FdsDemo.maneuversArrayController.selection'
            contentValueKey: 'epoch'
            rowHeight: 21
      topLeftMinThickness: 150
      topLeftMaxThickness: 250
      dividerView: SC.SplitDividerView.design
         layout: {}
      bottomRightView: Flot.GraphView.design
         layout:
            centerX: 0
            centerY: 0
            width:   400
            height:  400 
         dataBinding: SC.Binding.from 'FdsDemo.maneuverController.data'
