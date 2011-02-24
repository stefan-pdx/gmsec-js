FdsDemo.mainPage = SC.Page.design
   mainPane: SC.MainPane.design
      childViews: 'topView tabView bottomView'.w()
      topView: SC.ToolbarView.design
         layout:
            top: 0
            left: 0
            right: 0
            height: 36
         childViews: 'titleView settingsButton infoButton'.w()
         anchorLocation: SC.ANCHOR_TOP
         titleView: SC.LabelView.design
            layout:
               centerY: 0 
               left: 12 
               height: 24
               width: 600
            controlSize: SC.LARGE_CONTROL_SIZE
            value: 'Online FDS Demo'
            fontWeight: SC.BOLD_WEIGHT
         settingsButton: SC.ButtonView.design
            layout:
               centerY: 0
               height: 24
               right: 100
               width: 100
            title: 'Settings'
            icon: 'sc-icon-options-16'
         infoButton: SC.ButtonView.design
            layout:
               centerY: 0
               height: 24
               right: 12
               width: 80
            title: 'Info'
            icon: 'sc-icon-info-16'
            target: 'FdsDemo.infoController'
            action: 'showInfoPane'
      tabView: SC.TabView.design
         layout:
            left: 0
            right: 0
            top: 24
            bottom: 36 
         nowShowing: 'FdsDemo.welcome.mainView'
         items: [ { title: 'Driver',              value: 'FdsDemo.driver.mainView' } 
                  { title: 'Simulation',          value: 'FdsDemo.simulation.mainView' }
                  { title: 'Maneuver Planning',   value: 'FdsDemo.maneuverPlanning.mainView' }
                  { title: 'Orbit Determination', value: 'FdsDemo.orbitDetermination.mainView' }
                  { title: 'GMSEC Activity',      value: 'FdsDemo.gmsecActivity.mainView' } ]
         itemTitleKey: 'title'
         itemValueKey: 'value'
         userDefaultKey: 'mainPane'
      bottomView: SC.ToolbarView.design
         layout:
            bottom: 0
            left: 0
            right: 0
            height: 36
         childViews: 'titleView'.w()
         anchorLocation: SC.ANCHOR_BOTTOM
         titleView: SC.LabelView.design
            layout:
               centerY: 0
               right: 20
               height: 24
               width: 1024
            controlSize: SC.LARGE_CONTROL_SIZE
            value: 'Site & GMSEC Integration by Stefan Novak, FreeFlyer support from Dave Conkey'
            textAlign: SC.ALIGN_RIGHT
