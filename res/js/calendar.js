var CALENDAR = function () {
  var wrap, label, months = ["Janvier", "Fevrier", "Mars", "Avril", "May", "Juin", "Juillet", "Aout", "Septembre", "Octobre", "Novembre", "Decembre"];

  function init (newWrap) {
    wrap = $(newWrap || "#main-layout-calendar");
    label = wrap.find("#label");

    wrap.find ("#prev").bind ("click.calendar", function () {
                                                  switchMonth (false);
                                                });

    wrap.find ("#next").bind ("click.calendar", function () {
                                                  switchMonth (true);
                                                });

    label.click ();
  }

  function switchMonth (next, month, year) {
    var current = label.text ().trim ().split (" ");
    var tempYear = parseInt (curr[1], 10);

    if (next) {
      if (curr[0] === "Decembre") {
        month = month || 0;
        year = tempYear + 1;
      } else {
        month = month || months.indexOf (curr[0] + 1);
        year = tempYear;
      }
    } else {
      if (curr[0] === "Janvier") {
        month = month || 11;
        year = tempYear -1;
      } else {
        month = month || months.indexOf (curr[0] - 1);
        year = tempYear;
      }
    }

    calendar =  createCal(year, month);
    $("#main-layout-calendar-frame", wrap) 
      .find(".curr") 
      .removeClass("curr") 
      .addClass("temp") 
      .end() 
      .prepend(calendar.calendar()) 
      .find(".temp") 
      .fadeOut("slow", function () {
                         $(this).remove(); }); 
 
    $('#label').text(calendar.label);

//    {
//      calendar : function () { /* returns a jquery object of the calendar */ }, 
//      label    : "Month Year" 
//    }
  }

  function createCalendar (year, month) {

    var day = 1, i, j;
    var haveDay = true;

    var startDay = new Date (year, month, day).getDay ();
    var daysInMonth = [31, (((year % 4 == 0) && (year % 100) != 0)) || (year % 400) ? 29 : 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31];
    var calendar = [];

    if (createCalendar.cache[year]) {
      if (createCalendar.cache[year][month]) {
        return createCalendar.cache[year][month];
      } else {
        return createCalendar.cache[year];
      }
    }

    i = 0;
    while (haveDay) {
      calendar[i] = [];
      for (j = 0; j < 7; j++) {
        if (i === 0) {
          if (j === startDay) {
            calendar[i][j] = day++;
            startDay++;
          }
        } else if (day <= dayInMonth[month]) {
          calendar[i][j] = day++;
        } else {
          calendar[i][j] = "";
          haveDay = false;
        }
      }
      i++;
    }

    if (calendar[5]) {
      for (i = 0; i < calendar[5].length; i++) {
        if (calendar[5][i] !== "") {
          calendar[4][i] = "<span>"+calendar[4][i]+"</span><span>"+calendar[5][i]+"</span>";
        }
      }
    }

    for (i = 0; i < calendar.length; i++) {
      calendar[i] = "<tr><td>"+calendar[i].join ("</td><td>")+"</td></tr>";
    }

    calendar = $("<table>"+calendar.join ("")+"</table>").addClass ("curr");
    $("td:empty", calendar).addClass ("nil");

    if (month === new Date ().getMonth ()) {
      $('td', calendar).filter (function () {
                                  return $(this).text () == new Date ().getDate ().toString ();
                                }).addClass ("today");
    }

    createCalendar.cache[year][month] = {calendar : function () {
                                                      return calendar.clone () }, label : months[month]+" "+year};

    return createCalendar.cache[year][month];
  }

  createCalendar.cache = {};
  return {
    init : init,
    switchMonth : switchMonth,
    createCalendar : createCalendar
  };
};
